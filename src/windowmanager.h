#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "petroules-utilities-global.h"

// Platform-specific headers for private functions
#ifdef Q_WS_WIN
#include <windows.h>
#endif

class NativeWindowInfo;

/*!
    \class WindowManager

    Provides various methods for interacting with the window manager.
 */
class PETROULES_UTILITIES_API WindowManager
{
    /*!
        Specifies which windows to include in generated lists.

        The WindowListOptionIncludingWindow and WindowListExcludeDesktopElements constants may be combined with
        the others by adding or ORing them together and passing the resulting value to the appropriate function.

        Only windows from the current session can be retrieved. You cannot retrieve windows from other active user
        sessions running on the system.
     */
    enum WindowListOptions
    {
        /*!
            List all windows, including both onscreen and offscreen windows. When retrieving a list with
            this option, the \a relativeToWindow parameter should be set to \a nullWindowId().

            On Mac OS X, this value corresponds directly to kCGWindowListOptionAll.
         */
        WindowListOptionAll = 0,

        /*!
            List all windows that are currently onscreen. Windows are returned in order from front to back.
            When retrieving a list with this option, the \a relativeToWindow parameter should be set to \a nullWindowId().

            On Mac OS X, this value corresponds directly to kCGWindowListOptionOnScreenOnly.
         */
        WindowListOptionOnScreenOnly = (1 << 0),

        /*!
            List all windows that are currently onscreen and in front of the window specified in the \a relativeToWindow
            parameter. Windows are returned in order from front to back.

            On Mac OS X, this value corresponds directly to kCGWindowListOptionOnScreenAboveWindow.
         */
        WindowListOptionOnScreenAboveWindow = (1 << 1),

        /*!
            List all windows that are currently onscreen and in behind the window specified in the \a relativeToWindow
            parameter. Windows are returned in order from front to back.

            On Mac OS X, this value corresponds directly to kCGWindowListOptionOnScreenBelowWindow.
         */
        WindowListOptionOnScreenBelowWindow = (1 << 2),

        /*!
            Include the window specified by the \a relativeToWindow parameter in the returned list. You must combine
            this option with the WindowListOptionOnScreenAboveWindow or kCGWindowListOptionOnScreenBelowWindow option
            to retrieve meaningful results.

            On Mac OS X, this value corresponds directly to kCGWindowListOptionIncludingWindow.
         */
        WindowListOptionIncludingWindow = (1 << 3),

        /*!
            Exclude any windows from the list that are elements of the desktop, including the background picture and
            desktop icons. You may combine this option with the other options.

            On Mac OS X, this value corresponds directly to kCGWindowListExcludeDesktopElements.
         */
        WindowListExcludeDesktopElements = (1 << 4)
    };

    /*!
        Specifies options for capturing the image of a window.
     */
    enum WindowImageTypes
    {
        /*!
            When the capture rectangle is empty, using this option captures the entire window
            plus the area required to display any framing effects, such as the window shadow.

            On Mac OS X, this corresponds directly to kCGWindowImageDefault.
         */
        WindowImageDefault = 0,

        /*!
            When the capture rectangle is empty, using this option captures the window area only
            and does not capture the area occupied by any window framing effects (but includes the title bar).

            On Mac OS X, this corresponds directly to kCGWindowImageBoundsIgnoreFraming.
         */
        WindowImageBoundsIgnoreFraming = (1 << 0),

        /*!
            When capturing the window, partially transparent areas are backed by a solid white color so that
            the resulting image is fully opaque. This option can be combined with others.
         */
        WindowImageShouldBeOpaque = (1 << 1),

        /*!
            When capturing the window, only the shadow effects are captured.
         */
        WindowImageOnlyShadows = (1 << 2)
    };

public:
    // Cross-platform functions
    static void centerMainWindow(QMainWindow *const mainWindow);
    static bool isTopMost(QWidget *const window);
    static void setTopMost(QWidget *const window, bool topMost);
    static bool defaultFilterFunction(WId handle);

    static WId nullWindowId();
    static QRect nullRect();
    static QRect infiniteRect();
    static qint64 widToInteger(WId handle);
    static WId integerToWid(qint64 handle);

    // Functions whose implementation will vary per-platform but whose signatures will remain the same
    static QPixmap getCompositeImage(WId windowId, WindowManager::WindowListOptions listOptions = WindowManager::WindowListOptionIncludingWindow, WindowManager::WindowImageTypes imageOptions = WindowManager::WindowImageDefault, QRect bounds = nullRect());
    static QRect getWindowRect(WId windowId);
    static QRect getClientRect(WId windowId);
    static QString getWindowText(WId windowId);
    static WId rootWindow();
    static QList<NativeWindowInfo*> windowList(WindowManager::WindowListOptions listOptions = WindowManager::WindowListOptionAll, WId relativeToWindow = rootWindow());
    static WId windowFromPoint(const QPoint &p, WId relativeToWindow = rootWindow(), bool (*filterFunction)(WId handle) = defaultFilterFunction);
    static uint idleTime();
    static WId findWindow(const QString &name);
    static WId activeWindow();

private:
    static void updateTopMostFlags(QWidget *const window, bool topMost);
    static void setTopMostDefault(QWidget *const window, bool topMost);

    // Private functions present ONLY on the specified platforms
#ifdef Q_WS_WIN
    static BOOL CALLBACK processWindow(WId windowId, LPARAM lParam);
#endif

#ifdef Q_WS_MAC
    static void setTopMostMac(QWidget *const window, bool topMost);
#endif
};

#endif // WINDOWMANAGER_H
