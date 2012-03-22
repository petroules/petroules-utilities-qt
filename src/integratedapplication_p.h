#ifndef INTEGRATEDAPPLICATION_P_H
#define INTEGRATEDAPPLICATION_P_H

#include "mac/cocoainitializer.h"
#include "integratedapplication.h"

class objc_object;

class IntegratedApplication::Private
{
public:
    void constructorObjc();
    void destructorObjc();
    void setupCocoaEventHandler();

#ifdef Q_WS_MAC
    CocoaInitializer *cocoa;
    QMenuBar *macAppMenuBar;
    objc_object *eventMonitor;
    objc_object *dockIconClickEventHandler;
    IntegratedApplication *integratedApplication;
#endif

    static QString unixName;
    static QString bundleId;
    static QVersion applicationVersion;
    static QVersion fileVersion;
    static QString copyright;
    static QString trademarks;
    static QMap<IntegratedApplication::ApplicationUrl, QUrl> urls;
};

#endif // INTEGRATEDAPPLICATION_P_H
