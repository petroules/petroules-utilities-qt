#include "qversion.h"

/*!
    \struct QVersion
    The QVersion class represents a software version number.

    Version numbers consist of two to four components in the format major.minor[.build[.revision]]. The square
    brackets indicate optional components - the first two components, major and minor, are required, and the
    build and revision are optional (but if revision is present, build must also be).

    Each component is normally used according to the following convention:

    \list
        \i Major: Usually changed when software receives a very large update that may break backwards compatibility.
        \i Minor: Usually changed when software receives a significant but non-breaking change.
        \i Build: Usually changed to indicate a service release of software containing small enhancements or bug fixes.
        \i Revision: Used for various purposes or not at all. Sometimes used to differentiate between individual builds
        of the same version for testing purposes.
    \endlist

    The rules which govern whether a QVersion is considered "valid" are as follows: all components must always
    be greater than or equal to zero. There are two exceptions to this rule. If revision is -1, the version number
    is not considered to include revision at all. Similarly, if both revision and build are -1, the version number
    is not considered to include either of them. Any other combination (such as if build is -1 but revision is not
    or any of the components are less than 0 save for the two exceptions above) constitutes an invalid version number.

    This class is designed to be compatible with the \c System.Version class from the Microsoft .NET Framework.

    See http://msdn.microsoft.com/en-us/library/system.version.aspx
 */

/*!
    Constructs a new QVersion using the values of \a major, \a minor, \a build, and \a revision to create the version number.

    \param major The major component of the version.
    \param minor The minor component of the version.
    \param build The build component of the version - optional.
    \param revision The revision component of the version - optional.
 */
QVersion::QVersion(long major, long minor, long build, long revision, QObject *parent)
    : QObject(parent), m_major(major), m_minor(minor), m_build(build), m_revision(revision)
{
}

/*!
    Constructs a new QVersion by parsing \a version. If the parse does not result in a valid QVersion,
    the components are initialized to -1, -1, -1, -1, which is an invalid QVersion.

    \param version The string representing a version number to parse into a QVersion structure.
 */
QVersion::QVersion(const QString &version, QObject *parent)
    : QObject(parent), m_major(-1), m_minor(-1), m_build(-1), m_revision(-1)
{
    bool ok;
    QVersion version2 = QVersion::parse(version, &ok);
    if (ok && version2.isValid())
    {
        QVersion::copy(this, version2);
    }
}

/*!
    Constructs a new QVersion by copying the data of \a version.

    \param version The other QVersion instance from which to copy data.
 */
QVersion::QVersion(const QVersion &version)
    : QObject()
{
    QVersion::copy(this, version);
}

/*!
    Parses a QVersion from a string.

    The string must be in the format major.minor.build.revision, major.minor.build, or major.minor.
    The method will also set \a ok to \c true or \c false to indicate that \a version was successfully
    parsed into a QVersion, or whether it failed.

    \param version The string to parse into a version.
    \param[out] ok Whether a valid QVersion was parsed.
 */
QVersion QVersion::parse(const QString &version, bool *ok)
{
    long num;
    long num2;
    QVersion result;

    QStringList strArray = version.split(".");
    int length = strArray.length();
    if (length < 2 || length > 4)
    {
        if (ok)
        {
            *ok = false;
        }

        return QVersion();
    }

    if (!tryParseComponent(strArray[0], &num))
    {
        if (ok)
        {
            *ok = false;
        }

        return QVersion();
    }

    if (!tryParseComponent(strArray[1], &num2))
    {
        if (ok)
        {
            *ok = false;
        }

        return QVersion();
    }

    length -= 2;
    if (length > 0)
    {
        long num3;
        if (!tryParseComponent(strArray[2], &num3))
        {
            if (ok)
            {
                *ok = false;
            }

            return QVersion();
        }

        length--;
        if (length > 0)
        {
            long num4;
            if (!tryParseComponent(strArray[3], &num4))
            {
                if (ok)
                {
                    *ok = false;
                }

                return QVersion();
            }

            result = QVersion(num, num2, num3, num4);
        }
        else
        {
            result = QVersion(num, num2, num3);
        }
    }
    else
    {
        result = QVersion(num, num2);
    }

    if (ok)
    {
        *ok = true;
    }

    return result;
}

/*!
    \internal

    Parses a component of a version number and returns whether it was successfully converted to a valid integer.

    If the conversion succeeds, \a parsedComponent will be set to the value of the component and the method
    will return \a true provided the component's value was greater than or equal to 0. Otherwise, \a parsedComponent
    will be set to 0 and the method will return \a false.
 */
bool QVersion::tryParseComponent(const QString &component, long *parsedComponent)
{
    bool ok;
    *parsedComponent = component.toLong(&ok);
    return ok && *parsedComponent >= 0;
}

/*!
    Returns a simplified copy of this QVersion.

    This property strips all redundant zeroes at the end of a version
    number so that it is compact as possible. For example, 1.0.0 will
    become version 1.0. This method is guaranteed to return a valid
    QVersion.
 */
QVersion QVersion::simplified() const
{
    // Copy constructor will copy it
    QVersion copy = *this;

    // 1.0.0 is the same as 1.0, etc...
    if (copy.m_revision <= 0)
    {
        copy.m_revision = -1;

        if (copy.m_build <= 0)
        {
            copy.m_build = -1;

            if (copy.m_minor < 0)
            {
                copy.m_minor = 0;

                if (copy.m_major < 0)
                {
                    copy.m_major = 0;
                }
            }
        }
    }

    Q_ASSERT(copy.isValid());

    return copy;
}

/*!
    \property QVersion::valid
    This property holds whether the QVersion is valid.

    A QVersion is valid if its major and minor components are greater than or equal
    to 0 and its build and revision numbers are greater than or equal to -1. Also,
    if the QVersion has a value for the revision field (greater than -1), it must
    also have a value for the build field.
 */

/*!
    \internal
 */
bool QVersion::isValid() const
{
    // Minimum values for each component no matter what
    bool basicValid = this->m_major >= 0 && this->m_minor >= 0 && this->m_build >= -1 && this->m_revision >= -1;

    // If revision has a value, then build must
    if (this->m_revision >= -1)
    {
        return basicValid && this->m_build >= -1;
    }

    return basicValid;
}

/*!
    Compares this QVersion to \a other and returns a value representing their logical relation.

    The return value will be -1 if this QVersion is considered less than \a other, 1 if this QVersion
    is considered greater than \a other, and 0 if this QVersion and \a other are considered equal.

    Note that if this QVersion or \a other is not valid this method will still perform a "logical" comparison.
 */
long QVersion::compareTo(const QVersion &other) const
{
    // If the major versions differ, we know the result right away
    if (this->m_major > other.m_major)
    {
        return 1;
    }
    else if (this->m_major < other.m_major)
    {
        return -1;
    }

    // If the major versions were identical, the minor should tell us...
    if (this->m_minor > other.m_minor)
    {
        return 1;
    }
    else if (this->m_minor < other.m_minor)
    {
        return -1;
    }

    // Both the major and minor components were indentical, so build will definitely tell us
    if (this->m_build > other.m_build)
    {
        return 1;
    }
    else if (this->m_build < other.m_build)
    {
        return -1;
    }

    // All the previous components were identical, so this gives the final say
    if (this->m_revision > other.m_revision)
    {
        return 1;
    }
    else if (this->m_revision < other.m_revision)
    {
        return -1;
    }

    // Every component of each was identical, so they are the same
    return 0;
}

/*!
    Determines whether this QVersion is equal to \a other.

    QVersions whose individual build components all match are considered equal.
    For example, 1.0.0.0 is equal to 1.0.0.0.
 */
bool QVersion::equals(const QVersion &other) const
{
    return this->m_major == other.m_major && this->m_minor == other.m_minor &&
        this->m_build == other.m_build && this->m_revision == other.m_revision;
}

/*!
    Converts this object to a QString with each integer component separated by a dot.

    Normally, the result is a string in the format major.minor.build.revision. However,
    if the build and revision numbers are -1, only the first two components will be returned,
    and if the revision number is -1, only the first three components will be returned. If the
    QVersion is not valid (this takes precedence over all other cases), an empty string is returned.
 */
QString QVersion::toString() const
{
    // If invalid, we get blankness...
    if (!this->isValid())
    {
        return QString();
    }

    if (this->m_build == -1)
    {
        return this->toString(2);
    }

    if (this->m_revision == -1)
    {
        return this->toString(3);
    }

    return this->toString(4);
}

/*!
    Converts this object to a QString with each integer component separated by a dot,
    up to the number of fields indicated by \a fieldCount.

    If \a fieldCount is 1, the major component is returned as a simple string. If it is 2,
    the major and minor components are returned separated by a dot. If it is 3, the major,
    minor and build components are returned separated by a dot. If it is 4, the major, minor,
    build and revision components are returned separated by a dot.

    If a value is passed to \a fieldCount that would result in more fields than are available
    being returned, or \a fieldCount is less than or equal to 0, the result is an empty string.

    If the QVersion is not valid, an empty string is returned.

    \param The number of fields to return.
 */
QString QVersion::toString(int fieldCount) const
{
    // If it's an invalid version, we get blankness...
    if (!this->isValid())
    {
        return QString();
    }

    // If we don't want any components, we get an empty string, if one, just the major and if 2, the major and minor
    switch (fieldCount)
    {
        case 0:
            return QString();
        case 1:
            return QString::number(this->m_major);
        case 2:
            return QString("%1.%2").arg(this->m_major).arg(this->m_minor);
    }

    // If the field count was not 0, 1, or 2, and there is no build number,
    // then we have an invalid case so return an empty string
    if (this->m_build == -1)
    {
        return QString();
    }

    // If the field count was 3, give the first 3 components
    if (fieldCount == 3)
    {
        return QString("%1.%2.%3").arg(this->m_major).arg(this->m_minor).arg(this->m_build);
    }

    // If the field count was not between 0 and 3 inclusive, and revision
    // is not present, then we have an invalid case so return an empty string
    if (this->m_revision == -1)
    {
        return QString();
    }

    // Anything other than 4 here is illogical so return an empty string if that's the case.
    if (fieldCount != 4)
    {
        return QString();
    }

    // Finally we return a string with all 4 components since the field count must be 4
    return QString("%1.%2.%3.%4").arg(this->m_major).arg(this->m_minor).arg(this->m_build).arg(this->m_revision);
}

/*!
    \property QVersion::major
    This property holds the major component of the version.

    The major component is the most significant component of a version and is always required.
 */
long QVersion::major() const
{
    return this->m_major;
}

/*!
    \property QVersion::minor
    This property holds the minor component of the version.

    The minor component is the second most significant component of a version and is always required.
 */
long QVersion::minor() const
{
    return this->m_minor;
}

/*!
    \property QVersion::build
    This property holds the build component of the version.

    The build component is the third most (second least) significant component of a version. It is optional,
    but only if revision is also omitted.
 */
long QVersion::build() const
{
    return this->m_build;
}

/*!
    \property QVersion::revision
    This property holds the revision component of the version.

    The revision component is the fourth most (least) significant component of a version. It is optional.
 */
long QVersion::revision() const
{
    return this->m_revision;
}

/*!
    Performs a copy of the data in \a source to \a destination.

    If \a source and \a destination both refer to the same object, the method
    returns immediately without copying anything.
 */
void QVersion::copy(QVersion *const destination, const QVersion &source)
{
    // No sense in copying the same thing to itself
    if (destination == &source)
    {
        return;
    }

    destination->m_major = source.m_major;
    destination->m_minor = source.m_minor;
    destination->m_build = source.m_build;
    destination->m_revision = source.m_revision;
}

QVersion& QVersion::operator=(const QVersion &version)
{
    QVersion::copy(this, version);
    return *this;
}

bool operator==(const QVersion &v1, const QVersion &v2)
{
    return v1.equals(v2);
}

bool operator!=(const QVersion &v1, const QVersion &v2)
{
    return !v1.equals(v2);
}

bool operator<(const QVersion &v1, const QVersion &v2)
{
    return v1.compareTo(v2) < 0;
}

bool operator<=(const QVersion &v1, const QVersion &v2)
{
    return v1.compareTo(v2) <= 0;
}

bool operator>(const QVersion &v1, const QVersion &v2)
{
    return v1.compareTo(v2) > 0;
}

bool operator>=(const QVersion &v1, const QVersion &v2)
{
    return v1.compareTo(v2) >= 0;
}
