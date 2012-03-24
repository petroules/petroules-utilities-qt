#ifndef MACHELPERS_H
#define MACHELPERS_H

#include <QString>
#import <Cocoa/Cocoa.h>
class QWidget;

QString qt_mac_NSStringToQString(const NSString *nsstr);
NSString* qt_mac_QStringToNSString(const QString &string);
NSWindow* qt_mac_windowForWidget(const QWidget *widget);

#endif // MACHELPERS_H
