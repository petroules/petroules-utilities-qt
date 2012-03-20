#ifndef WINDOWSFILEREGISTRATION_H
#define WINDOWSFILEREGISTRATION_H

#include "petroules-utilities-global.h"

class PETROULESUTILITIESSHARED_EXPORT WindowsFileRegistration
{
public:
    WindowsFileRegistration();
    bool areFileExtensionsRegistered(const QString &progID);
    bool registerFileExtensions(const QString &progID, const QStringList &extensions, bool doRegister = true);
    inline bool unregisterFileExtensions(const QString &progID, const QStringList &extensions) { return registerFileExtensions(progID, extensions, false); }

private:
    bool registerProgID(const QString &progID, bool doRegister);
    bool registerToHandleExt(const QString &progID, const QString &ext, bool doRegister);
};

#endif // WINDOWSFILEREGISTRATION_H
