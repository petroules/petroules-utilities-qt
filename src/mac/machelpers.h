#ifndef MACHELPERS_H
#define MACHELPERS_H

#include <QString>

#ifdef __OBJC__
#define OBJC_CLASS(classname) @class classname
#else
#define OBJC_CLASS(classname) typedef struct objc_object classname
#endif

class QWidget;
OBJC_CLASS(NSString);
OBJC_CLASS(NSWindow);

QString qt_mac_NSStringToQString(const NSString *nsstr);
NSString* qt_mac_QStringToNSString(const QString &string);
NSWindow* qt_mac_windowForWidget(const QWidget *widget);

#endif // MACHELPERS_H
