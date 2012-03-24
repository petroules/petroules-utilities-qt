#include "machelpers.h"
#include <QVarLengthArray>
#include <QtGui/QWidget>
#import <AppKit/AppKit.h>

QString qt_mac_NSStringToQString(const NSString *nsstr)
{
    CFStringRef str = reinterpret_cast<const CFStringRef>(nsstr);
    if (!str)
    {
        return QString();
    }

    CFIndex length = CFStringGetLength(str);
    const UniChar *chars = CFStringGetCharactersPtr(str);
    if (chars)
    {
        return QString(reinterpret_cast<const QChar*>(chars), length);
    }

    QVarLengthArray<UniChar> buffer(length);
    CFStringGetCharacters(str, CFRangeMake(0, length), buffer.data());
    return QString(reinterpret_cast<const QChar*>(buffer.constData()), length);
}

NSString* qt_mac_QStringToNSString(const QString &string)
{
    CFStringRef ref = CFStringCreateWithCharacters(0, reinterpret_cast<const UniChar*>(string.unicode()), string.length());
    return [reinterpret_cast<const NSString*>(ref) autorelease];
}

NSWindow* qt_mac_windowForWidget(const QWidget *widget)
{
    return [(NSView*)widget->winId() window];
}

// https://developer.apple.com/library/mac/#documentation/General/Conceptual/MOSXAppProgrammingGuide/FullScreenApp/FullScreenApp.html
// https://bugreports.qt.nokia.com/browse/QTBUG-21607

void qt_mac_setLionFullscreenEnabled(const QWidget *widget, bool enabled)
{
    [qt_mac_windowForWidget(widget) setCollectionBehavior: (enabled ? NSWindowCollectionBehaviorFullScreenPrimary : NSWindowCollectionBehaviorDefault)];
}

bool qt_mac_isLionFullscreen(const QWidget *widget)
{
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
    return [qt_mac_windowForWidget(widget) styleMask] & NSFullScreenWindowMask;
#else
    return false;
#endif
}

void qt_mac_toggleLionFullscreen(const QWidget *widget, bool on)
{
    bool current = qt_mac_isLionFullscreen(widget);

    // If the value we want differs from the current, toggle to obtain the desired setting
    if (current != on)
    {
        [qt_mac_windowForWidget(widget) toggleFullScreen: nil];
    }
}
