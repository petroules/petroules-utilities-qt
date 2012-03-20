#include "linuxsysteminfo.h"

/*!
    Gets the native package manager for the specified Linux distribution.
 */
LinuxSystemInfo::PackageManager LinuxSystemInfo::packageManagerFor(LinuxDistribution distribution)
{
    switch (distribution)
    {
        case Fedora:
        case RedHat:
            return RedHatPM;
        case Debian:
        case Ubuntu:
        case Kubuntu:
        case Xubuntu:
            return DebianPM;
        case Gentoo:
            return PortagePM;
        case Slackware:
            return SlackwarePM;
        default:
            // TODO: The rest need to be filled in!
            return UnknownPM;
    }
}

#ifdef Q_OS_LINUX
/*!
    Gets the currently running desktop environment.
 */
LinuxSystemInfo::DesktopEnvironment LinuxSystemInfo::desktopEnvironment()
{
    if (LinuxSystemInfo::processId("gnome-session"))
    {
        return GNOME;
    }
    else if (LinuxSystemInfo::processId("ksmserver"))
    {
        return KDE;
    }
    else if (LinuxSystemInfo::processId("xfce-mcs-manage"))
    {
        return Xfce;
    }
    else
    {
        return UnknownDesktopEnvironment;
    }
}

/*!
    Gets the name of the running Linux distribution.
 */
LinuxSystemInfo::LinuxDistribution LinuxSystemInfo::currentDistribution()
{
    QString description = LinuxSystemInfo::description();
    if (description.contains("ubuntu") /* TODO: Make case insensitive */)
    {
        // TODO: Need a way to distinguish between the 3 *buntus
    }
    else if (description.contains("suse"))
    {
        // TODO: How to distinguish between SUSE and openSUSE?
    }

    // If we can't detect the Linux distribution, we return an unknown...
    return UnknownDistribution;
}

/*!
    Gets the name of the native package manager of the running Linux distribution.
 */
LinuxSystemInfo::PackageManager LinuxSystemInfo::currentPackageManager()
{
    return LinuxSystemInfo::packageManagerFor(LinuxSystemInfo::currentDistribution());
}

/*!
    Gets the code name of the running Linux distribution release.
 */
QString LinuxSystemInfo::codeName()
{
    return LinuxSystemInfo::lsbReleaseOutput("-c");
}

/*!
    Gets a description of the running Linux distribution, usually containing
    the name, release version and processor architecture.
 */
QString LinuxSystemInfo::description()
{
    return LinuxSystemInfo::lsbReleaseOutput("-d");
}

/*!
    Gets the distributor ID of the running Linux distribution,
    usually the same as or similar to the name.
 */
QString LinuxSystemInfo::distributorId()
{
    return LinuxSystemInfo::lsbReleaseOutput("-i");
}

/*!
    Gets the release version of the running Linux distribution.
 */
QString LinuxSystemInfo::version()
{
    return LinuxSystemInfo::lsbReleaseOutput("-r");
}

/*!
    Gets the output of the Linux lsb_release process.

    \param options The options that should be passed to lsb_release.
 */
QString LinuxSystemInfo::lsbReleaseOutput(const QString &options)
{
    return LinuxSystemInfo::processOutput("lsb_release", options);
}

/*!
    Gets the process ID of the specified process.

    \param process The process to check.
 */
int LinuxSystemInfo::processId(const QString &process)
{
    return LinuxSystemInfo::processOutput("pidof", process).toInt();
}
#endif

/*!
    Gets the output of the specified process as a string.

    \param process The name of the process to run.
    \param options The options to pass to the process.
 */
QString LinuxSystemInfo::processOutput(const QString &process, const QString &options)
{
    QProcess proc;
    proc.start(process, QStringList(options));
    proc.waitForFinished();
    return QString(proc.readAll());
}
