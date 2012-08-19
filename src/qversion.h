#ifndef QVERSION_H
#define QVERSION_H

#include "petroules-utilities-global.h"

struct PETROULESUTILITIESSHARED_EXPORT QVersion : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool valid READ isValid)
    Q_PROPERTY(long major READ major)
    Q_PROPERTY(long minor READ minor)
    Q_PROPERTY(long build READ build)
    Q_PROPERTY(long revision READ revision)

public:
    QVersion(long major = 0, long minor = 0, long build = -1, long revision = -1, QObject *parent = NULL);
    QVersion(const QString &version, QObject *parent = NULL);
    QVersion(const QVersion &version);
    static QVersion parse(const QString& version, bool *ok = NULL);
    QVersion simplified() const;
    bool isValid() const;
    long compareTo(const QVersion &other) const;
    bool equals(const QVersion &other) const;
    QString toString() const;
    QString toString(int fieldCount) const;
    long major() const;
    long minor() const;
    long build() const;
    long revision() const;

    /*!
        Included for compatibility with Microsoft's \c System.Version class in the .NET Framework.

        See http://msdn.microsoft.com/en-us/library/system.version.majorrevision.aspx
     */
    inline int majorRevision() const { return (qint16)(this->m_revision >> 16); }

    /*!
        Included for compatibility with Microsoft's \c System.Version class in the .NET Framework.

        See http://msdn.microsoft.com/en-us/library/system.version.minorrevision.aspx
     */
    inline int minorRevision() const { return (qint16)(this->m_revision & 65535); }

    QVersion& operator=(const QVersion &version);
    PETROULESUTILITIESSHARED_EXPORT friend bool operator==(const QVersion &v1, const QVersion &v2);
    PETROULESUTILITIESSHARED_EXPORT friend bool operator!=(const QVersion &v1, const QVersion &v2);
    PETROULESUTILITIESSHARED_EXPORT friend bool operator<(const QVersion &v1, const QVersion &v2);
    PETROULESUTILITIESSHARED_EXPORT friend bool operator<=(const QVersion &v1, const QVersion &v2);
    PETROULESUTILITIESSHARED_EXPORT friend bool operator>(const QVersion &v1, const QVersion &v2);
    PETROULESUTILITIESSHARED_EXPORT friend bool operator>=(const QVersion &v1, const QVersion &v2);

private:
    inline static void copy(QVersion *const destination, const QVersion &source);
    static bool tryParseComponent(const QString &component, long *parsedComponent);

    long m_major;
    long m_minor;
    long m_build;
    long m_revision;
};

#endif // QVERSION_H
