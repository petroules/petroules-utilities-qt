#include "windowmanager.h"
#include "nativewindowinfo.h"
#include <limits>

/*!
    Centers a main window in the middle of the desktop on the default screen.
 */
void WindowManager::centerMainWindow(QMainWindow *const mainWindow)
{
    QRect rect = mainWindow->frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    mainWindow->move(rect.topLeft());
}

bool WindowManager::isTopMost(QWidget *const window)
{
    return (window->windowFlags() & Qt::WindowStaysOnTopHint) == Qt::WindowStaysOnTopHint;
}

/*!
    Sets the window to stay on top of other windows. On Windows, this has the added benefit
    of causing the window not to be hidden and reshown.

    \param useDefault If \c true, uses the default Qt way of changing the top-most property
    (which will hide and show the window), regardless of the current platform.
 */
void WindowManager::setTopMost(QWidget *const window, bool topMost)
{
    if (!window || topMost == WindowManager::isTopMost(window))
    {
        return;
    }

#ifdef Q_WS_WIN
    HWND z = topMost ? HWND_TOPMOST : HWND_NOTOPMOST;
    if (SetWindowPos(window->winId(), z, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE))
    {
        WindowManager::updateTopMostFlags(window, topMost);
    }
#elif defined(Q_WS_MAC)
    WindowManager::setTopMostMac(window, topMost);
#else
    setTopMostDefault(window, topMost);
#endif
}

void WindowManager::updateTopMostFlags(QWidget *const window, bool topMost)
{
    Qt::WindowFlags flags = window->windowFlags();
    if (topMost)
    {
        window->overrideWindowFlags(flags | Qt::WindowStaysOnTopHint);
    }
    else
    {
        window->overrideWindowFlags(flags & ~Qt::WindowStaysOnTopHint);
    }
}

/*!
    Sets the top-most property for a window using the Qt way, which hides and re-shows the window, causing a flickering effect.
 */
void WindowManager::setTopMostDefault(QWidget *const window, bool topMost)
{
    if (!window || topMost == WindowManager::isTopMost(window))
    {
        return;
    }

    Qt::WindowFlags flags = window->windowFlags();
    if (topMost)
    {
        window->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
        window->show();
    }
    else
    {
        window->setWindowFlags(flags ^ Qt::WindowStaysOnTopHint);
        window->show();
    }
}

/*!
    The default filter function, which simply returns true for any handle.
 */
bool WindowManager::defaultFilterFunction(WId handle)
{
    Q_UNUSED(handle);

    return true;
}

QRect WindowManager::nullRect()
{
    return QRect();
}

QRect WindowManager::infiniteRect()
{
    return QRect(0, 0, std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
}

qint64 WindowManager::widToInteger(WId handle)
{
#ifdef Q_WS_WIN
    return reinterpret_cast<int>(handle);
#elif defined(Q_OS_SYMBIAN)
    // TODO: WId is defined as a CCoeControl pointer
    return 0;
#else
    return handle;
#endif
}

WId WindowManager::integerToWid(qint64 handle)
{
#ifdef Q_WS_WIN
    return reinterpret_cast<WId>(static_cast<int>(handle));
#elif defined(Q_OS_SYMBIAN)
    // TODO: WId is defined as a CCoeControl pointer
    return 0;
#else
    return static_cast<WId>(handle);
#endif
}

WId WindowManager::findWindow(const QString &name)
{
#ifdef Q_WS_WIN
    return FindWindowW(NULL, (LPCWSTR)name.utf16());
#else
    foreach (NativeWindowInfo *info, windowList())
    {
        if (info->windowName() == name)
        {
            return info->windowId();
        }
    }

    return nullWindowId();
#endif
}

/*!
    WINDOWS:
    Returns the window at the specified point, starting the search at the specified window.
    The desktop window is used if none is specified, and only visible and non-iconic windows
    are searched, and windows whose handles return true by the passed filter function.
 */
WId WindowManager::windowFromPoint(const QPoint &p, WId relativeToWindow, bool(*filterFunction)(WId))
{
#ifdef Q_WS_WIN
    // If we're passed a null handle, this means something probably went wrong, so just return the root window
    if (!relativeToWindow)
    {
        return WindowManager::rootWindow();
    }

    // Create a list of window IDs
    QList<WId> *windowIds = new QList<WId>();

    // Here's where we get a little messy. We need to pass our filter function to the enumeration
    // callback, but the callback takes just one parameter... so we'll use a QPair object to pack
    // our window list and filter function pointer.
    QPair<void*, void*> *pair = new QPair<void*, void*>();
    pair->first = windowIds;
    pair->second = (void*)filterFunction;

    // Enumerate the child windows of this process, passing it the address of our window ID list
    // so that the enumeration callback can insert each of the child windows' handles into it, as
    // well as the filter function (since they are both encapsulated in our QPair)
    EnumChildWindows(relativeToWindow, &processWindow, (LPARAM)pair);

    // If this window actually had any child windows...
    if (windowIds->length() > 0)
    {
        // ...then go through them and...
        for (int i = 0; i < windowIds->length(); i++)
        {
            WId winId = windowIds->at(i);

            // ...check if each child's rect contains our point; if so, check its children as well
            if (getWindowRect(winId).contains(p))
            {
                return windowFromPoint(p, winId, filterFunction);
            }
        }

        // If not, just return the window itself
        return relativeToWindow;
    }
    else
    {
        // If it's got no child windows, the window itself is our match
        return relativeToWindow;
    }
#elif defined(Q_WS_MAC) || defined(Q_WS_X11)
    QList<NativeWindowInfo*> windows = windowList(WindowListOptionAll, relativeToWindow);
#ifdef Q_WS_MAC // Windows are listed topmost first, this is convenient :)
    for (int i = 0; i < windows.size(); i++)
#elif defined(Q_WS_X11) // For X11 we iterate backwards :D
    for (int i = windows.size() - 1; i >= 0; i--)
#endif
    {
        NativeWindowInfo *info = windows.at(i);
        if (info->bounds().contains(p))
        {
            return info->windowId();
        }
    }

    Q_UNUSED(filterFunction);
    return rootWindow();
#else
    return nullWindowId();
#endif
}
