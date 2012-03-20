#ifndef PLATFORMINFORMATION_H
#define PLATFORMINFORMATION_H

#include "petroules-utilities-global.h"
#include <QtCore>

class PETROULESUTILITIESSHARED_EXPORT PlatformInformation : public QObject
{
    Q_OBJECT

public:
    static QString osString();
    static QString compilerString();
    static QString byteOrder();
    static int wordSize();

private:
    PlatformInformation() { }
};

#endif // PLATFORMINFORMATION_H
