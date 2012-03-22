#ifndef MACHELPERS_H
#define MACHELPERS_H

#include <QString>
#import <Cocoa/Cocoa.h>

QString qt_mac_NSStringToQString(const NSString *nsstr);
NSString* qt_mac_QStringToNSString(const QString &string);

#endif // MACHELPERS_H
