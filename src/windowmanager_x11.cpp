#include "windowmanager.h"

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>

QRect WindowManager::getWindowRect(WId windowId)
{
    qDebug() << "getWindowRect not yet implemented.";
    return QRect();
}

QRect WindowManager::getClientRect(WId windowId)
{
    qDebug() << "getWindowRect not yet implemented.";
    return QRect();
}

QString WindowManager::getWindowText(WId windowId)
{

}

WId WindowManager::rootWindow()
{

}

WId WindowManager::windowFromPoint(const QPoint &p, WId parent, bool(*filterFunction)(WId))
{
// NOTE: ALL THIS IS UNTESTED
    int screen = QCursor::x11Screen();
    int unused;

    int x = p.x();
    int y = p.y();
    Window target;
    if (!XTranslateCoordinates(XOpenDisplay(NULL), QX11Info::appRootWindow(screen), QX11Info::appRootWindow(screen), x, y, &unused, &unused, &target))
    {
        return 0;
    }

    if (!target || target == QX11Info::appRootWindow(screen))
    {
        return 0;
    }

    return target;
}
#endif
