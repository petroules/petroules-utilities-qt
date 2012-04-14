#ifndef INTEGRATEDAPPLICATION_P_H
#define INTEGRATEDAPPLICATION_P_H

#include "integratedapplication.h"
#ifdef Q_WS_MAC
#include "mac/cocoainitializer.h"

#ifdef __OBJC__
@class DockIconClickEventHandler;
#endif
#endif

class IntegratedApplication::Private
{
public:
#ifdef Q_WS_MAC
    void constructorObjc();
    void destructorObjc();
    void setupCocoaEventHandler();
    void setBadgeText_mac(const QString &text);

    CocoaInitializer *cocoa;
    QMenuBar *macAppMenuBar;
#ifdef __OBJC__
    DockIconClickEventHandler *dockIconClickEventHandler;
#endif
#endif

    IntegratedApplication *integratedApplication;

    static QString unixName;
    static QString bundleId;
    static QVersion applicationVersion;
    static QVersion fileVersion;
    static QString copyright;
    static QString trademarks;
    static QMap<IntegratedApplication::ApplicationUrl, QUrl> urls;
};

#endif // INTEGRATEDAPPLICATION_P_H
