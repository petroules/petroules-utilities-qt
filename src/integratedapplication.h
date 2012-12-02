#ifndef INTEGRATEDAPPLICATION_H
#define INTEGRATEDAPPLICATION_H

#include "petroules-utilities-global.h"
#include "inactivityeventfilter.h"
#include "qversion.h"
#include <QtSingleApplication>

class QMenuBar;
class QMenu;

class PETROULES_UTILITIES_API IntegratedApplication : public QtSingleApplication
{
    Q_OBJECT

public:
    /*!
        Enumerates URLs related to the application and its distributing organization.
     */
    enum ApplicationUrl
    {
        /*!
            Specifies the URL of the home page of the organization distributing the application.
         */
        OrganizationHomePage,
        
        /*!
            Specifies the URL at which donations may be submitted.
         */
        OrganizationDonations,
        
        /*!
            Specifies the URL of the home page of the application itself.
         */
        ApplicationHomePage,
        
        /*!
            Specifies the URL at which application help and support may be found.
         */
        ApplicationHelp,
        
        /*!
            Specifies the URL at which the application update definition file may be found.
         */
        ApplicationUpdate
    };
    
    IntegratedApplication(const QString &id, int &argc, char **argv, int = QT_VERSION);
    IntegratedApplication(int &argc, char **argv, bool GUIenabled, int = QT_VERSION);
    IntegratedApplication(int &argc, char **argv, Type, int = QT_VERSION);
    ~IntegratedApplication();
    inline static IntegratedApplication* instance() { return qobject_cast<IntegratedApplication*>(QtSingleApplication::instance()); }
    bool event(QEvent *event);
    QMenuBar* macApplicationMenuBar() const;
    void macSetDockMenu(QMenu *dockMenu);
    void setBadgeText(const QString &text);
    void setBadgeText(int number);
    void clearBadgeText();
    bool handleReopen();
    virtual bool handleReopen(bool hasVisibleWindows);

    static void preInitialization();
    
    static QString unixName();
    static void setUnixName(const QString &unixName);
    static QString bundleId();
    static void setBundleId(const QString &bundleId);
    static QVersion applicationQVersion();
    static void setApplicationVersion(const QVersion &applicationVersion);
    static QVersion fileVersion();
    static void setFileVersion(const QVersion &fileVersion);
    static QString copyright();
    static void setCopyright(const QString &copyright);
    static QString trademarks();
    static void setTrademarks(const QString &trademarks);
    static QUrl url(ApplicationUrl urlType);
    static void setUrl(ApplicationUrl urlType, QUrl url);
    static QString platformCode();
    static QString copyrightLink();
    
signals:
    void resetIdleTimer(QObject *object);

private:
    void construct();

private slots:
    void delayedConstruct();

private:
    class Private;
    Private *d;
};

// A convenient little macro for accessing our extended properties
#define qiApp (qobject_cast<IntegratedApplication*>(qApp))

#endif // INTEGRATEDAPPLICATION_H
