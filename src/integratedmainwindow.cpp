#include "integratedmainwindow.h"
#include "integratedmainwindow_p.h"
#include <limits>
#ifdef Q_WS_WIN
#include "win32/windowstaskbarintegration_includes.h"
#include "win32/desktopwindowmanager.h"
#include "win32/thumbbar.h"
#endif

#ifdef Q_WS_WIN
DEFINE_GUID(CLSID_TaskbarList,0x56fdf344,0xfd6d,0x11d0,0x95,0x8a,0x0,0x60,0x97,0xc9,0xa0,0x90);
DEFINE_GUID(IID_ITaskbarList,0x56FDF342,0xFD6D,0x11d0,0x95,0x8A,0x00,0x60,0x97,0xC9,0xA0,0x90);
DEFINE_GUID(IID_ITaskbarList2,0x602D4995,0xB13A,0x429b,0xA6,0x6E,0x19,0x35,0xE4,0x4F,0x43,0x17);
DEFINE_GUID(IID_ITaskbarList3,0xea1afb91,0x9e28,0x4b86,0x90,0xE9,0x9e,0x9f,0x8a,0x5e,0xef,0xaf);
DEFINE_GUID(IID_ITaskbarList4,0xc43dc798,0x95d1,0x4bea,0x90,0x30,0xbb,0x99,0xe2,0x98,0x3a,0x1a);
#endif

IntegratedMainWindow::Private::Private()
#ifdef Q_WS_WIN
    : m_taskbar(NULL), m_taskbarMessageId(0), m_thumbBarManager(NULL), moving(false)
#endif
{
}

IntegratedMainWindow::IntegratedMainWindow(QWidget *parent)
    : QMainWindow(parent), d(new Private())
{
    d->window = this;
#ifdef Q_WS_WIN
    d->m_thumbBarManager = new ThumbBar(this);
    d->m_taskbarMessageId = RegisterWindowMessage(TEXT("TaskbarButtonCreated"));

    if (DesktopWindowManager::isCompositionEnabled())
    {
        DesktopWindowManager::extendFrameIntoClientArea(this);
    }
#elif defined(Q_WS_X11)
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_NoSystemBackground, false);
    QPalette pal = this->palette();
    QColor bg = pal.window().color();
    bg.setAlpha(180);
    pal.setColor(QPalette::Window, bg);
    this->setPalette(pal);
    this->ensurePolished(); // workaround Oxygen filling the background
    this->setAttribute(Qt::WA_StyledBackground, false);
#endif
}

IntegratedMainWindow::~IntegratedMainWindow()
{
#ifdef Q_WS_WIN
    if (d->m_taskbar)
    {
        // Release our reference to the task bar and uninitialize
        d->m_taskbar->Release();
        d->m_taskbar = NULL;
        CoUninitialize();
    }
#endif
}

#ifdef Q_WS_WIN
/*!
    Handles communications between the window and its taskbar button as well as DWM functions.
 */
bool IntegratedMainWindow::winEvent(MSG *message, long *result)
{
    switch (message->message)
    {
        case WM_COMMAND:
            return d->m_thumbBarManager->winEvent(message, result);
        case WM_DWMSENDICONICTHUMBNAIL:
            d->m_maxThumbnailSize = QSize(HIWORD(message->lParam), LOWORD(message->lParam));
            sendThumbnailInternal();
            break;
        case WM_DWMSENDICONICLIVEPREVIEWBITMAP:
            sendPreviewInternal();
            break;
        default:
        {
            if (message->message == d->m_taskbarMessageId)
            {
                this->initialize(result);
                return true;
            }

            break;
        }
    }

    return QMainWindow::winEvent(message, result); // TODO: was 'return false'
}
#endif

void IntegratedMainWindow::mousePressEvent(QMouseEvent *event)
{
    QMainWindow::mousePressEvent(event);
#ifdef Q_WS_WIN
    // TODO: Only if blur behind's on           For menu  vvvvv
    if ((event->button() == Qt::LeftButton) /*&& !actionAt(event->pos())*/)
    {
        d->moving = true;
        d->offset = event->globalPos() - frameGeometry().topLeft();
    }
#endif
}

void IntegratedMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QMainWindow::mouseMoveEvent(event);
#ifdef Q_WS_WIN
    // TODO: Only if blur behind's on
    if (d->moving)
    {
        move(event->globalPos() - d->offset);
        //this->offset = event->globalPos();
    }
#endif
}

void IntegratedMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QMainWindow::mouseReleaseEvent(event);
#ifdef Q_WS_WIN
    // TODO: Only if blur behind's on
    if (event->button() == Qt::LeftButton)
    {
        d->moving = false;
    }
#endif
}

bool IntegratedMainWindow::taskbarExtensionsSupported() const
{
#ifdef Q_WS_WIN
    return QSysInfo::WindowsVersion >= QSysInfo::WV_WINDOWS7;
#else
    return false;
#endif
}

bool IntegratedMainWindow::dwmSupported() const
{
#ifdef Q_WS_WIN
    return QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA;
#else
    return false;
#endif
}

/*!
    Applies an overlay to the taskbar button of the given window handle to indicate
    application status or a notification to the user.

    \a pixmap icon to apply as the overlay icon.
    \a accessibilityDescription provides an alt text version of the information conveyed by the overlay for accessibility purposes
 */
bool IntegratedMainWindow::setOverlayIcon(const QIcon &icon, const QString &accessibilityDescription)
{
#ifdef Q_WS_WIN
    if (!icon.isNull())
    {
        return setOverlayIcon(icon.pixmap(IntegratedMainWindow::overlayIconSize()), accessibilityDescription);
    }
#else
    Q_UNUSED(icon);
    Q_UNUSED(accessibilityDescription);
#endif

    return false;
}

/*!
    Applies an overlay to the taskbar button of the given window handle to indicate
    application status or a notification to the user.

    \a pixmap image to apply as the overlay icon.
    \a accessibilityDescription provides an alt text version of the information conveyed by the overlay for accessibility purposes
 */
bool IntegratedMainWindow::setOverlayIcon(const QPixmap &pixmap, const QString &accessibilityDescription)
{
#ifdef Q_WS_WIN
    if (!pixmap.isNull())
    {
        return setOverlayIcon(pixmap.toWinHICON(), accessibilityDescription);
    }
#else
    Q_UNUSED(pixmap);
    Q_UNUSED(accessibilityDescription);
#endif

    return false;
}

#ifdef Q_WS_WIN
/*!
    Applies an overlay to the taskbar button of the given window handle to indicate
    application status or a notification to the user.
	
	This method is only available on Windows.

    \a hIcon handle to the overlay icon to apply.
    \a accessibilityDescription provides an alt text version of the information conveyed by the overlay for accessibility purposes
 */
bool IntegratedMainWindow::setOverlayIcon(HICON hIcon, const QString &accessibilityDescription)
{
    if (!d->m_taskbar)
    {
        return false;
    }

    if (hIcon)
    {
        HRESULT hr = d->m_taskbar->SetOverlayIcon(winId(), hIcon, accessibilityDescription.toStdWString().c_str());
        DestroyIcon(hIcon);
        return SUCCEEDED(hr);
    }

    return false;
}
#endif

/*!
    Removes the overlay icon from the taskbar button.
 */
bool IntegratedMainWindow::clearOverlayIcon()
{
#ifdef Q_WS_WIN
    if (!d->m_taskbar)
    {
        return false;
    }

    return SUCCEEDED(d->m_taskbar->SetOverlayIcon(winId(), NULL, NULL));
#else
    return false;
#endif
}

/*!
    Displays or updates a progress bar hosted in a taskbar button of the main application window
    to show the specific percentage completed of the full operation.

    \a value value from 0 to 1 that indicates the proportion of the operation that has been completed at the time the method is called.
 */
bool IntegratedMainWindow::setProgressValue(float value)
{
    // windef.h does this
#if defined(Q_CC_MSVC) && defined(max)
#error "Please define NOMINMAX in your project settings to avoid a compiler error."
#endif

    // Despite takig a ULONGLONG, a regular signed 32-bit integer is the maximum "maximum" value for the progress bar
    const int max = std::numeric_limits<int>::max();
    return setProgressValue(value * max, max);
}

/*!
    Displays or updates a progress bar hosted in a taskbar button of the main application window
    to show the specific percentage completed of the full operation.

    \a value an application-defined value that indicates the proportion of the operation that has been completed at the time the method is called.
    \a max an application-defined value that specifies the value \a value will have when the operation is complete.
 */
bool IntegratedMainWindow::setProgressValue(int value, int max)
{
    return setProgressValue(value, 0, max);
}

/*!
    Displays or updates a progress bar hosted in a taskbar button of the main application window
    to show the specific percentage completed of the full operation.

    \a value an application-defined value that indicates the proportion of the operation that has been completed at the time the method is called.
    \a min an application-defined value that indicates the value \a value had before the operation started.
    \a max an application-defined value that specifies the value \a value will have when the operation is complete.
 */
bool IntegratedMainWindow::setProgressValue(int value, int min, int max)
{
#ifdef Q_WS_WIN
    if (!d->m_taskbar)
    {
        return false;
    }

    return SUCCEEDED(d->m_taskbar->SetProgressValue(winId(), value, max - min));
#else
    Q_UNUSED(value);
    Q_UNUSED(min);
    Q_UNUSED(max);
    return false;
#endif
}

/*!
    Sets the type and state of the progress indicator displayed on a taskbar button of the window.

    \a state progress state of the progress button.
 */
bool IntegratedMainWindow::setProgressState(TaskbarProgressState state)
{
#ifdef Q_WS_WIN
    if (!d->m_taskbar)
    {
        return false;
    }

    // Validate the taskbar progress state so we can't pass an invalid value
    switch (state)
    {
        case NoProgress:
        case Indeterminate:
        case Normal:
        case Error:
        case Paused:
            break;
        default:
            return false;
    }

    return SUCCEEDED(d->m_taskbar->SetProgressState(winId(), static_cast<TBPFLAG>(state)));
#else
    Q_UNUSED(state);
    return false;
#endif
}

/*!
    Enables or disables iconic previews for the window.

    \a enable true to enable, false to disable and use the system default images.
 */
void IntegratedMainWindow::setIconicPreviewEnabled(bool enable)
{
#ifdef Q_WS_WIN
    BOOL fForceIconic = enable ? TRUE : FALSE;
    BOOL fHasIconicBitmap = enable ? TRUE : FALSE;

    DwmSetWindowAttribute(this->winId(), DWMWA_FORCE_ICONIC_REPRESENTATION, &fForceIconic, sizeof(fForceIconic));
    DwmSetWindowAttribute(this->winId(), DWMWA_HAS_ICONIC_BITMAP, &fHasIconicBitmap, sizeof(fHasIconicBitmap));
#else
    Q_UNUSED(enable);
#endif
}

bool IntegratedMainWindow::drawWindowFrameForIconicPreviews() const
{
    return d->m_windowFrameForIconicThumbnails;
}

void IntegratedMainWindow::setDrawWindowFrameForIconicPreviews(bool draw)
{
    d->m_windowFrameForIconicThumbnails = draw;
}

QPixmap IntegratedMainWindow::sendThumbnail()
{
    return QPixmap();
}

/*!
    Forces Windows to refresh the cached pixmap it received from \a sendPreview.
 */
void IntegratedMainWindow::invalidateThumbnail()
{
#ifdef Q_WS_WIN
    DwmInvalidateIconicBitmaps(winId());
#endif
}

QPixmap IntegratedMainWindow::sendPreview()
{
    return QPixmap::grabWindow(this->winId());
}

#ifdef Q_WS_WIN
/*!
    Sends the image that will appear as the window's iconic thumbnail.

    The image should not be larger than 200 by 108 pixels but will be
    scaled down preserving aspect ratio if necessary. To use the Windows
    default iconic thumbnail (an image of the window itself) pass a
    null pixmap.
 */
void IntegratedMainWindow::sendThumbnailInternal()
{
    QPixmap pix = sendThumbnail();
    if (!pix.isNull())
    {
        // If our image is larger than Windows said it could be in winEvent, scale it down
        if (pix.width() > d->m_maxThumbnailSize.width() || pix.height() > d->m_maxThumbnailSize.height())
        {
            pix = pix.scaled(d->m_maxThumbnailSize, Qt::KeepAspectRatio);
        }

        // Assert that our image does not exceed either of Windows' requested maximum dimensions
        Q_ASSERT(pix.width() <= d->m_maxThumbnailSize.width() && pix.height() <= d->m_maxThumbnailSize.height());

        // Convert our pixmap to Windows bitmap and send it to the DWM
        HBITMAP hBitmap = pix.toWinHBITMAP();
        DwmSetIconicThumbnail(winId(), hBitmap, d->m_windowFrameForIconicThumbnails ? DWM_SIT_DISPLAYFRAME : 0);
        if (hBitmap)
        {
            DeleteObject(hBitmap);
        }
    }
}

void IntegratedMainWindow::sendPreviewInternal()
{
    QPixmap pix = sendPreview();
    if (!pix.isNull())
    {
        HBITMAP hBitmap = pix.toWinHBITMAP();
        DwmSetIconicLivePreviewBitmap(winId(), hBitmap, 0, d->m_windowFrameForIconicThumbnails ? DWM_SIT_DISPLAYFRAME : 0);
        if (hBitmap)
        {
            DeleteObject(hBitmap);
        }
    }
}
#endif

/*!
    If \a fullscreen is true, marks this window to be treated by the shell as a full screen window.
    If \a fullscreen is false, the shell will automatically detect whether the window should be
    treated as fullscreen.
 */
bool IntegratedMainWindow::markFullscreenWindow(bool fullscreen)
{
#ifdef Q_WS_WIN
    if (!d->m_taskbar)
    {
        return false;
    }

    return SUCCEEDED(d->m_taskbar->MarkFullscreenWindow(winId(), fullscreen));
#else
    Q_UNUSED(fullscreen);
    return false;
#endif
}

ThumbBar* IntegratedMainWindow::thumbBarManager() const
{
    return d->m_thumbBarManager;
}

bool IntegratedMainWindow::isNativeFullScreenEnabled() const
{
#ifdef Q_WS_MAC
    return d->isNativeFullScreenEnabled_mac();
#else
    return false;
#endif
}

void IntegratedMainWindow::setNativeFullScreenEnabled(bool enable)
{
#ifdef Q_WS_MAC
    d->setNativeFullScreenEnabled_mac(enable);
#else
    Q_UNUSED(enable);
#endif
}

bool IntegratedMainWindow::isNativeFullScreen() const
{
#ifdef Q_WS_MAC
    return d->isNativeFullScreen_mac();
#else
    return false;
#endif
}

void IntegratedMainWindow::setNativeFullScreen(bool set)
{
#ifdef Q_WS_MAC
    return d->setNativeFullScreen_mac(set);
#else
    Q_UNUSED(set);
#endif
}

bool IntegratedMainWindow::activateTab()
{
#ifdef Q_WS_WIN
    if (!d->m_taskbar)
    {
        return false;
    }

    return SUCCEEDED(d->m_taskbar->SetTabActive(NULL, winId(), 0));
#else
    return false;
#endif
}

#ifdef Q_WS_WIN
void IntegratedMainWindow::initialize(long *result)
{
    CoInitialize(NULL);
    HRESULT hRes = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, reinterpret_cast<LPVOID*>(&(d->m_taskbar)));
    if (SUCCEEDED(hRes))
    {
        d->m_taskbar->HrInit();
    }
    else
    {
        d->m_taskbar->Release();
        d->m_taskbar = NULL;
        CoUninitialize();
    }

    *result = hRes;
}

void DwmInvalidateIconicBitmaps(HWND hwnd)
{
    HMODULE dwmapi = LoadLibrary(TEXT("dwmapi.dll"));
    if (dwmapi)
    {
        t_DwmInvalidateIconicBitmaps invalidate_icon_bitmap = reinterpret_cast<t_DwmInvalidateIconicBitmaps>(GetProcAddress(dwmapi, "DwmInvalidateIconicBitmaps"));
        invalidate_icon_bitmap(hwnd);

        FreeLibrary(dwmapi);
    }
}

void DwmSetIconicThumbnail(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags)
{
    HMODULE dwmapi = LoadLibrary(TEXT("dwmapi.dll"));
    if (dwmapi)
    {
        t_DwmSetIconicThumbnail set_iconic_thumbnail = reinterpret_cast<t_DwmSetIconicThumbnail>(GetProcAddress(dwmapi, "DwmSetIconicThumbnail"));
        set_iconic_thumbnail(hwnd, hbmp, dwSITFlags);

        FreeLibrary(dwmapi);
    }
}

void DwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute)
{
    HMODULE dwmapi = LoadLibrary(TEXT("dwmapi.dll"));
    if (dwmapi)
    {
        t_DwmSetWindowAttribute set_window_attribute = reinterpret_cast<t_DwmSetWindowAttribute>(GetProcAddress(dwmapi, "DwmSetWindowAttribute"));
        set_window_attribute(hwnd, dwAttribute, pvAttribute, cbAttribute);

        FreeLibrary(dwmapi);
    }
}

void DwmSetIconicLivePreviewBitmap(HWND hwnd, HBITMAP hbmp, POINT *pptClient, DWORD dwSITFlags)
{
    HMODULE dwmapi = LoadLibrary(TEXT("dwmapi.dll"));
    if (dwmapi)
    {
        t_DwmSetIconicLivePreviewBitmap set_live_preview = reinterpret_cast<t_DwmSetIconicLivePreviewBitmap>(GetProcAddress(dwmapi, "DwmSetIconicLivePreviewBitmap"));
        set_live_preview(hwnd, hbmp, pptClient, dwSITFlags);

        FreeLibrary(dwmapi);
    }
}
#endif
