#ifndef JUMPLIST_H
#define JUMPLIST_H

#include "petroules-utilities-global.h"

/*!
    \class JumpListFile
    Represents a jump list file destination.
 */
struct PETROULES_UTILITIES_API JumpListFile
{
public:
    /*!
        Constructs a new JumpListFile that is empty.
     */
    JumpListFile()
    {
    }

    /*!
        Constructs a new JumpListFile for the file specified by \a path.
     */
    JumpListFile(const QString &path)
        : path(path)
    {
    }

    /*!
        Returns a value indicating whether this object is equal to \a other.
     */
    inline bool operator==(const JumpListFile &other) const
    {
        return this->path == other.path;
    }

    /*!
        The file path referred to by this object.
     */
    QString path;
};

/*!
    \struct JumpListLink
    Represents a jump list link or shortcut destination.
*/
struct PETROULES_UTILITIES_API JumpListLink
{
public:
    /*!
        Constructs a new JumpListLink that is empty.
     */
    JumpListLink()
        : iconIndex(0)
    {
    }

    /*!
        Returns a value indicating whether this object is equal to \a other.
     */
    inline bool operator==(const JumpListLink &other) const
    {
        return this->path == other.path && this->arguments == other.arguments;
    }

    /*!
        The command-line arguments of the destination.
     */
    QString arguments;

    /*!
        The index of the icon in \a iconPath.
     */
    int iconIndex;

    /*!
        The path to the file containing the icon of this link.
     */
    QString iconPath;

    /*!
        The path to the target file or object.
     */
    QString path;

    /*!
        The title of the link shown on the jump list.
     */
    QString title;

    /*!
        The tooltip of the link shown on the jump list.
     */
    QString toolTip;

    /*!
        The working directory of the destination.
     */
    QString workingDirectory;
};

/*!
    \class Destination
    Represents a jump list item that is either a separator, file (\a JumpListFile) or link (\a JumpListLink).
 */
class PETROULES_UTILITIES_API Destination
{
public:
    enum PETROULES_UTILITIES_API Type
    {
        Separator,
        File,
        Link
    };

    /*!
        Constructs a new jump list separator.
     */
    Destination()
        : mType(Separator)
    {
    }

    /*!
        Constructs a new jump list file item.
     */
    Destination(const JumpListFile &item)
        : mType(File), mFile(item)
    {
    }

    /*!
        Constructs a new jump list link item.
     */
    Destination(const JumpListLink &link)
        : mType(Link), mLink(link)
    {
    }

    /*!
        Returns the type of destination - separator, file or link.
     */
    Type type() const
    {
        return this->mType;
    }

    /*!
        Returns the encapsulated jump list file.
     */
    const JumpListFile& file() const
    {
        return this->mFile;
    }

    /*!
        Returns the encapsulated jump list link.
     */
    const JumpListLink& link() const
    {
        return this->mLink;
    }

    /*!
        Returns a value indicating whether the destination is a separator.
     */
    bool isSeparator() const
    {
        return this->mType == Separator;
    }

    /*!
        Returns a value indicating whether the destination is a file.
     */
    bool isFile() const
    {
        return this->mType == File;
    }

    /*!
        Returns a value indicating whether the destination is a link.
     */
    bool isLink() const
    {
        return this->mType == Link;
    }

    /*!
        Returns a value indicating whether this object is equal to \a other.
     */
    inline bool operator==(const Destination &other) const
    {
        if (this->type() == other.type())
        {
            switch (this->type())
            {
            case File:
                return this->file() == other.file();
            case Link:
                return this->link() == other.link();
            default:
                return true;
            }
        }

        return false;
    }

private:
    Type mType;
    JumpListFile mFile;
    JumpListLink mLink;
};

class PETROULES_UTILITIES_API JumpList
{
public:
    JumpList();
    ~JumpList();
    void appendFrequentCategory();
    void appendRecentCategory();
    void appendCustomCategory(const QString &name, const QList<Destination> &destinations);
    void appendTasks(const QList<Destination> &destinations);
    QList<Destination> frequentDestinations() const;
    QList<Destination> recentDestinations() const;
    int maximumShownDestinations() const;
    void removeAll();
    void abort();
    void save();

    void reportFileUsage(const QString &fileName);
    void reportLinkUsage(const JumpListLink &link);

private:
    class Private;
    Private *d;
};

#endif // JUMPLIST_H
