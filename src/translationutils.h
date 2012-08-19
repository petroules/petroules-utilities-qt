#ifndef TRANSLATIONUTILS_H
#define TRANSLATIONUTILS_H

#include "petroules-utilities-global.h"

/*!
    \class TranslationUtils

    The TranslationUtils class provides helper methods used in translating Qt applications
    to various languages.
 */
class PETROULESUTILITIESSHARED_EXPORT TranslationUtils
{
public:
    static QStringList findQmFiles();
    static QString languageCode(const QString &qmFile);
    static QString languageName(const QString &qmFile, const char *context = "MainWindow");

private:
    TranslationUtils() { }
};

#endif // TRANSLATIONUTILS_H
