#include "windowmanager.h"
#include "nativewindowinfo.h"

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>

QList<WId> x11_getWindowsForAtom(Atom prop)
{
    QList<WId> res;
    Atom type = 0;
    int format = 0;
    uchar* data = 0;
    ulong count;
    ulong after;
    Display* display = QX11Info::display();
    Window window = QX11Info::appRootWindow();
    if (XGetWindowProperty(display, window, prop, 0, 1024 * sizeof(Window) / 4, False, AnyPropertyType, &type, &format, &count, &after, &data) == Success)
    {
        Window* list = reinterpret_cast<Window*>(data);
        for (uint i = 0; i < count; i++)
        {
            res.append(list[i]);
        }

        if (data)
        {
            XFree(data);
        }
    }

    return res;
}

NativeWindowInfo* x11_wIdToNativeWindowInfo(WId windowId)
{
    NativeWindowInfo *info = new NativeWindowInfo();
    info->setWindowId(windowId);
    info->setWindowName(WindowManager::getWindowText(windowId));
    info->setBounds(WindowManager::getWindowRect(windowId));
    return info;
}

QRect WindowManager::getWindowRect(WId windowId)
{
    int x, y;
    uint width, height, border, depth;
    Window root, child;
    Display* display = QX11Info::display();
    XGetGeometry(display, windowId, &root, &x, &y, &width, &height, &border, &depth);
    XTranslateCoordinates(display, windowId, root, x, y, &x, &y, &child);

    static Atom net_frame = 0;
    if (!net_frame)
    {
        net_frame = XInternAtom(QX11Info::display(), "_NET_FRAME_EXTENTS", True);
    }

    QRect rect(x, y, width, height);
    Atom type = 0;
    int format = 0;
    uchar* data = 0;
    ulong count, after;
    if (XGetWindowProperty(display, windowId, net_frame, 0, 4, False, AnyPropertyType, &type, &format, &count, &after, &data) == Success)
    {
        // _NET_FRAME_EXTENTS, left, right, top, bottom, CARDINAL[4]/32
        if (count == 4)
        {
            long* extents = reinterpret_cast<long*>(data);
            rect.adjust(-extents[0], -extents[2], extents[1], extents[3]);
        }

        if (data)
        {
            XFree(data);
        }
    }

    return rect;
}

QRect WindowManager::getClientRect(WId windowId)
{
    qDebug() << "getClientRect not yet implemented, returning window rect.";
    return getWindowRect(windowId);
}

QString WindowManager::getWindowText(WId windowId)
{
    QString name;
    char* str = 0;
    if (XFetchName(QX11Info::display(), windowId, &str))
    {
        name = QString::fromLatin1(str);
    }

    if (str)
    {
        XFree(str);
    }

    return name;
}

WId WindowManager::nullWindowId()
{
    return 0;
}

WId WindowManager::rootWindow()
{
    return QApplication::desktop()->winId();
}

QList<NativeWindowInfo*> WindowManager::windowList(WindowManager::WindowListOptions listOptions, WId relativeToWindow)
{
    // TODO: Need to implement
    Q_UNUSED(listOptions);
    Q_UNUSED(relativeToWindow);

    static Atom clients = 0;
    if (!clients)
    {
        clients = XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", True);
    }

    QList<WId> windowIds = x11_getWindowsForAtom(clients);
    QList<NativeWindowInfo*> windowInfo;
    foreach (WId wid, windowIds)
    {
        windowInfo.append(x11_wIdToNativeWindowInfo(wid));
    }

    return windowInfo;
}

QPixmap WindowManager::getCompositeImage(WId windowId, WindowManager::WindowListOptions listOptions, WindowManager::WindowImageTypes imageOptions, QRect bounds)
{
    // TODO: Implement this! For now we use default...
    Q_UNUSED(windowId);
    Q_UNUSED(listOptions);
    Q_UNUSED(imageOptions);
    Q_UNUSED(bounds);

    return QPixmap::grabWindow(windowId);
}

WId WindowManager::activeWindow()
{
    static Atom active = 0;
    if (!active)
    {
        active = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", True);
    }

    return x11_getWindowsForAtom(active).first();
}

// Thanks to https://bitbucket.org/libqxt/libqxt/src/1ddf7613e787/src/gui/qxtwindowsystem_x11.cpp for this
typedef struct {
    Window  window;     /* screen saver window - may not exist */
    int     state;      /* ScreenSaverOff, ScreenSaverOn, ScreenSaverDisabled*/
    int     kind;       /* ScreenSaverBlanked, ...Internal, ...External */
    unsigned long    til_or_since;   /* time til or since screen saver */
    unsigned long    idle;      /* total time since last user input */
    unsigned long   eventMask; /* currently selected events for this client */
} XScreenSaverInfo;

typedef XScreenSaverInfo* (*XScreenSaverAllocInfo)();
typedef Status (*XScreenSaverQueryInfo)(Display* display, Drawable* drawable, XScreenSaverInfo* info);

static XScreenSaverAllocInfo _xScreenSaverAllocInfo = 0;
static XScreenSaverQueryInfo _xScreenSaverQueryInfo = 0;

uint WindowManager::idleTime()
{
    static bool xssResolved = false;
    if (!xssResolved) {
        QLibrary xssLib(QLatin1String("Xss"), 1);
        if (xssLib.load()) {
            _xScreenSaverAllocInfo = (XScreenSaverAllocInfo) xssLib.resolve("XScreenSaverAllocInfo");
            _xScreenSaverQueryInfo = (XScreenSaverQueryInfo) xssLib.resolve("XScreenSaverQueryInfo");
            xssResolved = true;
        }
    }

    uint idle = 0;
    if (xssResolved)
    {
        XScreenSaverInfo* info = _xScreenSaverAllocInfo();
        const int screen = QX11Info::appScreen();
        Qt::HANDLE rootWindow = QX11Info::appRootWindow(screen);
        _xScreenSaverQueryInfo(QX11Info::display(), (Drawable*) rootWindow, info);
        idle = info->idle;
        if (info)
        {
            XFree(info);
        }
    }

    return idle;
}
#endif
