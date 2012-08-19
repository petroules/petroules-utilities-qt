#ifndef LINUXSYSTEMINFO_H
#define LINUXSYSTEMINFO_H

#include "petroules-utilities-global.h"

/*!
    \class LinuxSystemInfo

    Provides information on various aspects of the running Linux distribution.
 */
class PETROULES_UTILITIES_API LinuxSystemInfo
{
public:
    /*!
        \enum DesktopEnvironment

        Enumerates X11 desktop environments.
     */
    enum DesktopEnvironment
    {
        /*!
            Specifies the GNOME desktop environment.
         */
        GNOME,

        /*!
            Specifies the K desktop environment.
         */
        KDE,

        /*!
            Specifies the Xfce desktop environment.
         */
        Xfce,

        /*!
            Specifies an unknown desktop environment.
         */
        UnknownDesktopEnvironment
    };

    /*!
        \enum LinuxDistribution

        Enumerates Linux distributions.
     */
    enum LinuxDistribution
    {
        /*!
            Specifies Fedora Linux.
         */
        Fedora,

        /*!
            Specifies Debian Linux.
         */
        Debian,

        /*!
            Specifies Gentoo Linux.
         */
        Gentoo,

        /*!
            Specifies Kubuntu Linux.
         */
        Kubuntu,

        /*!
            Specifies Mandriva Linux.
         */
        Mandriva,

        /*!
            Specifies openSUSE Linux.
         */
        OpenSUSE,

        /*!
            Specifies RedHat Linux.
         */
        RedHat,

        /*!
            Specifies Slackware Linux.
         */
        Slackware,

        /*!
            Specifies Ubuntu Linux.
         */
        Ubuntu,

        /*!
            Specifies an unknown Linux distribution.
         */
        UnknownDistribution,

        /*!
            Specifies Xubuntu Linux.
         */
        Xubuntu
    };

    /*!
        \enum PackageManager
        Enumerates package managers used on Linux systems.
     */
    enum PackageManager
    {
        /*!
            Specifies dpkg, the package manager used on Debian and derivatives such as Ubuntu.
            It uses .deb files.
         */
        DebianPM,

        /*!
            Specifies the RPM Package Manager, used on Red Hat and derivatives such as Fedora.
            It uses .rpm files.
         */
        RedHatPM,

        /*!
            Specifies the Portage package manager, used on Gentoo.
         */
        PortagePM,

        /*!
            Specifies the Slackware package manager, used solely on Slackware.
            It uses .tgz and .txz files.
         */
        SlackwarePM,

        /*!
            Specifies an unknown package manager or a package manager
            whose identity could not be retrieved.
         */
        UnknownPM
    };

    static PackageManager packageManagerFor(LinuxDistribution distribution);
#ifdef Q_OS_LINUX
    static DesktopEnvironment desktopEnvironment();
    static LinuxDistribution currentDistribution();
    static PackageManager currentPackageManager();
    static QString codeName();
    static QString description();
    static QString distributorId();
    static QString version();

private:
    static QString lsbReleaseOutput(const QString &options);
    static int processId(const QString &process);
#endif
    static QString processOutput(const QString &process, const QString &options);
};

#endif // LINUXSYSTEMINFO_H
