#ifndef MACHELPERS_H
#define MACHELPERS_H

#include <QString>
class QWidget;
@class NSString;
@class NSWindow;

QString qt_mac_NSStringToQString(const NSString *nsstr);
NSString* qt_mac_QStringToNSString(const QString &string);
NSWindow* qt_mac_windowForWidget(const QWidget *widget);
void qt_mac_setLionFullscreenEnabled(const QWidget *widget, bool enabled);
bool qt_mac_isLionFullscreen(const QWidget *widget);
void qt_mac_toggleLionFullscreen(const QWidget *widget, bool on);

#endif // MACHELPERS_H
