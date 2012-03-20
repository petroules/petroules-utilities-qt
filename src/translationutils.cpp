#include "translationutils.h"

/*!
    Retrieves a list of all .qm files in the /tr virtual directory.
 */
QStringList TranslationUtils::findQmFiles()
{
    QDir dir(":/tr");
    QStringList fileNames = dir.entryList(QStringList("*.qm"), QDir::Files, QDir::Name);
    QMutableStringListIterator i(fileNames);
    while (i.hasNext())
    {
        i.next();
        i.setValue(dir.filePath(i.value()));
    }

    return fileNames;
}

/*!
    Gets a language code from the filename of a .qm file.

    The name of the .qm file should be formatted like: \code <appname>_<language-code>.qm \endcode

    \param qmFile The name of the .qm file to extract the language code from.
 */
QString TranslationUtils::languageCode(const QString &qmFile)
{
    // Our QM files are formatted like: appname_XX.qm,
    // so split by underscore and get the last part (XX.qm)
    QStringList list = qmFile.split("_", QString::SkipEmptyParts);
    QString code = list.last();

    // Remove the file extension and dot and return
    code.chop(3);
    return code;
}

/*!
    Gets the localized language name of a .qm file.

    For an English-language translation file, this method will return "English".
    For a German-language translation file it will return "Deutsch", and so on.

    \param qmFile The filename of the .qm file to extract the language name of.
 */
QString TranslationUtils::languageName(const QString &qmFile, const char *context)
{
    QTranslator translator;
    translator.load(qmFile);
    return translator.translate(context, "English");
}
