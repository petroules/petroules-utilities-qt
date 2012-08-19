#include "integratedapplication.h"
#include "integratedapplication_p.h"
#if defined(Q_WS_WIN)
#include "windows.h"
#include "win32/explorerstyle.h"
#elif defined(Q_WS_MAC)
#include "mac/cocoainitializer.h"
#endif

QString IntegratedApplication::Private::unixName = QString();
QString IntegratedApplication::Private::bundleId = QString();
QVersion IntegratedApplication::Private::applicationVersion = QVersion();
QVersion IntegratedApplication::Private::fileVersion = QVersion();
QString IntegratedApplication::Private::copyright = QString();
QString IntegratedApplication::Private::trademarks = QString();
QMap<IntegratedApplication::ApplicationUrl, QUrl> IntegratedApplication::Private::urls = QMap<IntegratedApplication::ApplicationUrl, QUrl>();

IntegratedApplication::IntegratedApplication(const QString &id, int &argc, char **argv, int)
    : QtSingleApplication(id, argc, argv), d(new Private())
{
    construct();
}

IntegratedApplication::IntegratedApplication(int &argc, char **argv, bool GUIenabled, int)
    : QtSingleApplication(argc, argv, GUIenabled), d(new Private())
{
    construct();
}

IntegratedApplication::IntegratedApplication(int &argc, char **argv, Type type, int)
    : QtSingleApplication(argc, argv, type), d(new Private())
{
    construct();
}

void IntegratedApplication::construct()
{
    d->integratedApplication = this;

#ifdef Q_WS_WIN
    // We need this call to allow the original process to bring itself to the foreground
    AllowSetForegroundWindow(ASFW_ANY);

    // Make the application look a little better on Windows (mostly affects toolbars and dock windows)
    qApp->setStyle(new ExplorerStyle());
#elif defined(Q_WS_MAC)
    d->cocoa = new CocoaInitializer();
    d->macAppMenuBar = new QMenuBar();

    // http://doc.qt.nokia.com/latest/qapplication.html#lastWindowClosed
    // http://doc.qt.nokia.com/latest/qapplication.html#quitOnLastWindowClosed-prop
    // default's true
    qApp->setQuitOnLastWindowClosed(false);
    qApp->setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

    // Event filter for our idle timer to detect when the user appears to have gone away...
    InactivityEventFilter *filter = new InactivityEventFilter(qApp);
    qApp->installEventFilter(filter);
    QObject::connect(filter, SIGNAL(resetIdleTimer(QObject*)), qApp, SIGNAL(resetIdleTimer(QObject*)));

#ifdef Q_WS_MAC
    // Mac OS X: This allows the application to recognize when the user double clicks a file in the Finder
    QObject::connect(qiApp, SIGNAL(fileOpenRequest(QString)), qiApp, SIGNAL(messageReceived(QString)));

    d->constructorObjc();

    // For some reason the execution of our setup method MUST be delayed
    QTimer::singleShot(1, this, SLOT(delayedConstruct()));
#endif
}

void IntegratedApplication::delayedConstruct()
{
#ifdef Q_WS_MAC
    d->setupCocoaEventHandler();
#endif
}

IntegratedApplication::~IntegratedApplication()
{
#ifdef Q_WS_MAC
    d->destructorObjc();

    if (d->cocoa)
        delete d->cocoa;

    if (d->macAppMenuBar)
        delete d->macAppMenuBar;
#endif

    if (this->d)
        delete this->d;
}

bool IntegratedApplication::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::ApplicationActivate:
        handleReopen();
        break;
    default:
        break;
    }

    return QtSingleApplication::event(event);
}

QMenuBar* IntegratedApplication::macApplicationMenuBar() const
{
#ifdef Q_WS_MAC
    return this->d->macAppMenuBar;
#else
    return NULL;
#endif
}

void IntegratedApplication::macSetDockMenu(QMenu *dockMenu)
{
#ifdef Q_WS_MAC
    extern void qt_mac_set_dock_menu(QMenu *);
    qt_mac_set_dock_menu(dockMenu);
#else
    Q_UNUSED(dockMenu);
#endif
}

void IntegratedApplication::setBadgeText(const QString &text)
{
#ifdef Q_WS_MAC
    d->setBadgeText_mac(text);
#else
    Q_UNUSED(text);
#endif
}

void IntegratedApplication::setBadgeText(int number)
{
    setBadgeText(QString::number(number));
}

void IntegratedApplication::clearBadgeText()
{
    setBadgeText(QString());
}

bool IntegratedApplication::handleReopen()
{
    bool hasVisibleWindows = false;
    foreach (QWidget *w, IntegratedApplication::topLevelWidgets())
    {
        if (w->isVisible())
        {
            hasVisibleWindows = true;
            break;
        }
    }

    return handleReopen(hasVisibleWindows);
}

bool IntegratedApplication::handleReopen(bool hasVisibleWindows)
{
    Q_UNUSED(hasVisibleWindows);
    return true;
}

QString IntegratedApplication::unixName()
{
    return IntegratedApplication::Private::unixName;
}

void IntegratedApplication::setUnixName(const QString &unixName)
{
    IntegratedApplication::Private::unixName = unixName;
}

/*!
    Gets the identifier of the Mac OS X application bundle.

    The identifier should be in reverse-DNS format, for example: \c com.mycompany.myapp.
 */
QString IntegratedApplication::bundleId()
{
    return IntegratedApplication::Private::bundleId;
}

void IntegratedApplication::setBundleId(const QString &bundleId)
{
    IntegratedApplication::Private::bundleId = bundleId;
}

QVersion IntegratedApplication::applicationQVersion()
{
    return IntegratedApplication::Private::applicationVersion;
}

void IntegratedApplication::setApplicationVersion(const QVersion &applicationVersion)
{
    IntegratedApplication::Private::applicationVersion = applicationVersion;
    QCoreApplication::setApplicationVersion(applicationVersion.toString());
}

QVersion IntegratedApplication::fileVersion()
{
    return IntegratedApplication::Private::fileVersion;
}

void IntegratedApplication::setFileVersion(const QVersion &fileVersion)
{
    IntegratedApplication::Private::fileVersion = fileVersion;
}

QString IntegratedApplication::copyright()
{
    return IntegratedApplication::Private::copyright;
}

void IntegratedApplication::setCopyright(const QString &copyright)
{
    IntegratedApplication::Private::copyright = copyright;
}

QString IntegratedApplication::trademarks()
{
    return IntegratedApplication::Private::trademarks;
}

void IntegratedApplication::setTrademarks(const QString &trademarks)
{
    IntegratedApplication::Private::trademarks = trademarks;
}

QUrl IntegratedApplication::url(ApplicationUrl urlType)
{
    return IntegratedApplication::Private::urls.value(urlType);
}

void IntegratedApplication::setUrl(ApplicationUrl urlType, QUrl url)
{
    IntegratedApplication::Private::urls.insert(urlType, url);
}

QString IntegratedApplication::platformCode()
{
#ifdef Q_OS_WIN
    return "windows";
#elif defined(Q_OS_MAC)
    return "macosx";
#elif defined(Q_OS_LINUX)
    return "linux";
#endif
}

QString IntegratedApplication::copyrightLink()
{
    return IntegratedApplication::copyright().replace(IntegratedApplication::organizationName(),
        QString("<a href=\"http://www.%1\">%2</a>")
            .arg(IntegratedApplication::organizationDomain())
            .arg(IntegratedApplication::organizationName()));
}
