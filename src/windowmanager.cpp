#include "windowmanager.h"

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
