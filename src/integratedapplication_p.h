#ifndef INTEGRATEDAPPLICATION_P_H
#define INTEGRATEDAPPLICATION_P_H

#include "integratedapplication.h"
#include "mac/machelpers.h"

class CocoaInitializer;
OBJC_CLASS(DockIconClickEventHandler);

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
    DockIconClickEventHandler *dockIconClickEventHandler;
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
