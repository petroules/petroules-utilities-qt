#include "windowmanager.h"
#include "nativewindowinfo.h"

#ifdef Q_WS_WIN
#include "dwmapi.h"

WId WindowManager::nullWindowId()
{
    return NULL;
}

QRect WindowManager::getWindowRect(WId windowId)
{
    RECT rect;
    if (GetWindowRect(windowId, &rect))
    {
        return QRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }
    else
    {
        return QRect();
    }
}

QRect WindowManager::getClientRect(WId windowId)
{
    RECT rect;
    if (GetClientRect(windowId, &rect))
    {
        return QRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }
    else
    {
        return QRect();
    }
}

/*!
    Gets the caption of the specified window. If the window has no caption, an empty string is returned.
 */
QString WindowManager::getWindowText(WId windowId)
{
    QString str;
    int windowTextLength = GetWindowTextLengthW(windowId);
    if (windowTextLength > 0)
    {
        // We need an extra character for the null terminator so we add 1
        LPWSTR buffer = (LPWSTR)malloc(sizeof(WCHAR) * (windowTextLength + 1));
        if (buffer)
        {
            if (GetWindowTextW(windowId, buffer, windowTextLength + 1))
            {
                str = QString::fromWCharArray(buffer);
            }

            free(buffer);
        }
    }

    return str;
}

WId WindowManager::rootWindow()
{
    return GetDesktopWindow();
}

class WindowEnumerator
{
public:
    explicit WindowEnumerator(WId parent = WindowManager::rootWindow(), bool (*filterFunction)(WId handle) = WindowManager::defaultFilterFunction)
    {
        // Because WindowEnumerator contains static variables due to the poor design
        // of EnumChildWindows, we must ensure we don't access this simultaneously
        mutex.lock();
        windows.clear();
        filter = filterFunction;
        EnumChildWindows(parent, enumChildWindowsProc, 0);
    }

    ~WindowEnumerator()
    {
        mutex.unlock();
    }

    static BOOL CALLBACK enumChildWindowsProc(WId handle, LPARAM param)
    {
        Q_UNUSED(param);

        if (filter(handle))
        {
            windows.append(handle);
        }

        return true;
    }

    static QList<WId> windowList()
    {
        return windows;
    }

private:
    QMutex mutex;
    static QList<WId> windows;
    static bool (*filter)(WId);
};

QList<WId> WindowEnumerator::windows;
bool (*WindowEnumerator::filter)(WId) = 0;

QList<NativeWindowInfo*> WindowManager::windowList(WindowListOptions listOptions, WId relativeToWindow)
{
    Q_UNUSED(listOptions); // TODO: Needs to be implemented

    // qxt likes this solutions - advantages and disadvantages?
    /*HDESK hdesk = ::OpenInputDesktop(0, false, DESKTOP_READOBJECTS);
    ::EnumDesktopWindows(hdesk, qxt_EnumWindowsProc, 0);
    ::CloseDesktop(hdesk);*/

#ifndef QT_NO_DEBUG
    QElapsedTimer timer;
    timer.start();
#endif
    WindowEnumerator enumerator(relativeToWindow);

    QList<NativeWindowInfo*> list;
    foreach (WId wid, enumerator.windowList())
    {
        NativeWindowInfo *info = new NativeWindowInfo();
        info->setWindowId(wid);
        info->setWindowName(getWindowText(wid));
        info->setBounds(getWindowRect(wid));
        list.append(info);
    }

#ifndef QT_NO_DEBUG
    qDebug() << "Took" << timer.elapsed() << "ms to enumerate window list";
#endif
    return list;
}

QPixmap WindowManager::getCompositeImage(WId windowId, WindowManager::WindowListOptions listOptions, WindowManager::WindowImageTypes imageOptions, QRect bounds)
{
    // NOTE: This will not capture window chrome
    //return QPixmap::grabWindow(windowId);

    // See https://code.google.com/p/aeroshot/ for hacks allowing capture of Aero glass
    RECT rect;
    if (!SUCCEEDED(DwmGetWindowAttribute(windowId, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof(RECT))))
    {
        // failed so Aero is probably disabled; we'll fall back to GetWindowRect
        GetWindowRect(windowId, &rect);
    }
    else
    {
        // 100px margin for window shadows; excess transparency will be trimmed later
        rect.left -= 100;
        rect.right += 100;
        rect.top -= 100;
        rect.bottom += 100;
    }

    HDC hSrc = CreateDC(TEXT("DISPLAY"), NULL, NULL, 0);
    HDC hDest = CreateCompatibleDC(hSrc);
    HBITMAP hBmp = CreateCompatibleBitmap(hSrc, rect.right - rect.left, rect.bottom - rect.top);
    HGDIOBJ hOldBmp = SelectObject(hDest, hBmp);
    BitBlt(hDest, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hSrc, rect.left, rect.top, SRCCOPY | CAPTUREBLT);
    QPixmap pixmap = QPixmap::fromWinHBITMAP(hBmp);
    SelectObject(hDest, hOldBmp);
    DeleteObject(hBmp);
    DeleteDC(hDest);
    DeleteDC(hSrc);
    return pixmap;
}

uint WindowManager::idleTime()
{
    LASTINPUTINFO info;
    info.cbSize = sizeof(LASTINPUTINFO);
    if (GetLastInputInfo(&info))
    {
        return GetTickCount() - info.dwTime;
    }

    return -1;
}

WId WindowManager::activeWindow()
{
    return GetForegroundWindow();
}

/*!
    The callback for EnumChildWindows; adds the window to the list
    if it's visible, non-iconic and matches our filter function.
 */
BOOL CALLBACK WindowManager::processWindow(WId windowId, LPARAM lParam)
{
    // lParam contains the pointer to our QPair, so let's cast that back
    QPair<void*, void*> *pair = (QPair<void*, void*>*)lParam;

    // The first object in the QPair was our list of window handles...
    QList<WId> *list = (QList<WId>*)pair->first;

    // ...and the second parameter was our filter function
    bool (*filter)(WId) = (bool (*)(WId))pair->second;

    // Apply our conditions and add the handle to the list
    if (IsWindowVisible(windowId) && !IsIconic(windowId) && filter(windowId))
    {
        list->append(windowId);
    }

    // Return true to continue iterating
    return true;
}
#endif
