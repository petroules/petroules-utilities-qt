#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "petroules-utilities-global.h"
#include <QtGui>

// Platform-specific headers for private functions
#ifdef Q_WS_WIN
#include <windows.h>
#endif

/*!
    \class WindowManager
    Provides various methods for interacting with the window manager.
 */
class PETROULESUTILITIESSHARED_EXPORT WindowManager
{
public:
    // Cross-platform functions
    static void centerMainWindow(QMainWindow *const mainWindow);
    static bool isTopMost(QWidget *const window);
    static void setTopMost(QWidget *const window, bool topMost);
    static bool defaultFilterFunction(WId handle);

    // Functions whose implementation will vary per-platform but whose signatures will remain the same
    static QRect getWindowRect(WId windowId);
    static QRect getClientRect(WId windowId);
    static QString getWindowText(WId windowId);
    static WId rootWindow();
    static WId windowFromPoint(const QPoint &p, WId parent = rootWindow(), bool (*filterFunction)(WId handle) = defaultFilterFunction);

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
