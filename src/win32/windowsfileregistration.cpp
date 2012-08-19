#include "windowsfileregistration.h"
#ifdef Q_WS_WIN
#include <qt_windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#endif

WindowsFileRegistration::WindowsFileRegistration()
{
}

bool WindowsFileRegistration::areFileExtensionsRegistered(const QString &progID)
{
#ifdef Q_WS_WIN
    QSettings settings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    return settings.contains(progID + "/CurVer/.");
#else
    Q_UNUSED(progID);
    return false;
#endif
}

bool WindowsFileRegistration::registerFileExtensions(const QString &progID, const QStringList &extensions, bool doRegister)
{
#ifdef Q_WS_WIN
    if (registerProgID(progID, doRegister))
    {
        foreach (QString ext, extensions)
        {
            if (!registerToHandleExt(progID, ext, doRegister))
            {
                return false;
            }
        }

        // Notify Windows that file associations have changed
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
        return true;
    }

    return false;
#else
    Q_UNUSED(progID);
    Q_UNUSED(extensions);
    Q_UNUSED(doRegister);
    return false;
#endif
}

bool WindowsFileRegistration::registerProgID(const QString &progID, bool doRegister)
{
#ifdef Q_WS_WIN
    QSettings settings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    if (!settings.isWritable())
    {
        return false;
    }

    QSettings::Status status = QSettings::NoError;
    if (doRegister)
    {
        settings.setValue(QString("%1/FriendlyTypeName").arg(progID), "Custom Jump List Document");
        settings.sync();
        if ((status = settings.status()) == QSettings::NoError)
        {
            // Find the application's path
            QString nativeExePath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());

            settings.setValue(QString("%1/DefaultIcon/.").arg(progID), nativeExePath + ",0");
            settings.sync();
            if ((status = settings.status()) == QSettings::NoError)
            {
                settings.setValue(QString("%1/CurVer/.").arg(progID), progID);
                settings.sync();
                if ((status = settings.status()) == QSettings::NoError)
                {
                    settings.setValue(QString("%1/shell/.").arg(progID), "Open");
                    settings.sync();
                    if ((status = settings.status()) == QSettings::NoError)
                    {
                        settings.setValue(QString("%1/shell/Open/Command/.").arg(progID), nativeExePath + " /HandleDocument:%1");
                        settings.sync();
                        status = settings.status();
                    }
                }
            }
        }
    }
    else
    {
        // Trigger failure by attempting to write so that not having permission returns false
        settings.setValue(QString("%1/CurVer/.").arg(progID), progID);
        settings.remove(progID);
        settings.sync();
        status = settings.status();
    }

    return status == QSettings::NoError;
#else
    Q_UNUSED(progID);
    Q_UNUSED(doRegister);
    return false;
#endif
}

bool WindowsFileRegistration::registerToHandleExt(const QString &progID, const QString &ext, bool doRegister)
{
#ifdef Q_WS_WIN
    QSettings settings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    if (!settings.isWritable())
    {
        return false;
    }

    QString key = QString("%1/OpenWithProgIDs/%2").arg(ext, progID);
    if (doRegister)
    {
        settings.setValue(key, QString());
    }
    else
    {
        settings.remove(key);
    }

    settings.sync();
    return settings.status() == QSettings::NoError;
#else
    Q_UNUSED(progID);
    Q_UNUSED(ext);
    Q_UNUSED(doRegister);
    return false;
#endif
}
