#include "machelpers.h"
#include <QVarLengthArray>
#include <QtGui/QWidget>

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
    return [reinterpret_cast<NSView*>(widget->winId()) window];
}
