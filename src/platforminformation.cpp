#include "platforminformation.h"

/*!
    Gets a string representing the name and version of the operating system on which the application is run.
 */
QString PlatformInformation::osString()
{
#if defined(Q_OS_WIN) && defined(Q_WS_WIN)
    switch (QSysInfo::WindowsVersion)
    {
#if QT_VERSION >= QT_VERSION_CHECK(4, 8, 3)
        case QSysInfo::WT_WINDOWS8:
            return "Windows NT 6.2 (8 or Server 2012)";
#endif
        case QSysInfo::WV_WINDOWS7:
            return "Windows NT 6.1 (7 or Server 2008 R2)";
        case QSysInfo::WV_VISTA:
            return "Windows NT 6.0 (Vista or Server 2008)";
        case QSysInfo::WV_2003:
            return "Windows NT 5.2 (Server 2003 or Server 2003 R2 or Windows Home Server or Windows XP Professional x64 Edition)";
        case QSysInfo::WV_XP:
            return "Windows NT 5.1 (XP)";
        case QSysInfo::WV_2000:
            return "Windows NT 5.0 (2000)";
        case QSysInfo::WV_NT:
            return "Windows NT 4.0";

        case QSysInfo::WV_Me:
            return "Windows Me";
        case QSysInfo::WV_98:
            return "Windows 98";
        case QSysInfo::WV_95:
            return "Windows 95";
        case QSysInfo::WV_32s:
            return "Windows 3.1";

        case QSysInfo::WV_CE_6:
            return "Windows CE 6.x";
        case QSysInfo::WV_CE_5:
            return "Windows CE 5.x";
        case QSysInfo::WV_CENET:
            return "Windows CE .NET";
        case QSysInfo::WV_CE:
            return "Windows CE";

        default:
            return "Windows";
    }
#elif defined(Q_OS_MAC) && defined(Q_WS_MAC)
    QStringList osxVersions;
    osxVersions << "Cheetah" << "Puma" << "Jaguar" << "Panther" << "Tiger" << "Leopard" << "Snow Leopard" << "Lion" << "Mountain Lion";
    
    switch (QSysInfo::MacintoshVersion)
    {
        case QSysInfo::MV_Unknown:
            return "Mac OS";
        case QSysInfo::MV_9:
            return "Mac OS 9";
        default:
            break;
    }
    
    int version = static_cast<int>(QSysInfo::MacintoshVersion) - static_cast<int>(QSysInfo::MV_10_0);
    if (version >= 0 && version < osxVersions.size())
        return QString("OS X 10.%1 %2").arg(version).arg(osxVersions.at(version));
    else
        return QString("OS X 10.%1").arg(version);
#elif defined(Q_OS_UNIX)
    #if defined(Q_OS_AIX)
        return "AIX";
    #elif defined(Q_OS_BSD4)
        #if defined(Q_OS_BSDI)
            return "BSD/OS";
        #elif defined(Q_OS_FREEBSD)
            return "FreeBSD";
        #elif defined(Q_OS_NETBSD)
            return "NetBSD";
        #elif defined(Q_OS_OPENBSD)
            return "OpenBSD";
        #else
            return "BSD 4.4";
        #endif
    #elif defined(Q_OS_CYGWIN)
        return "Cygwin";
    #elif defined(Q_OS_DGUX)
        return "DG/UX"
    #elif defined(Q_OS_DYNIX)
        return "DYNIX/ptx";
    #elif defined(Q_OS_HPUX)
        return "HP-UX";
    #elif defined(Q_OS_HURD)
        return "GNU Hurd";
    #elif defined(Q_OS_IRIX)
        return "SGI Irix";
    #elif defined(Q_OS_LINUX)
        return "Linux";
    #elif defined(Q_OS_LYNX)
        return "LynxOS";
    #elif defined(Q_OS_OSF)
        return "HP Tru64 UNIX";
    #elif defined(Q_OS_QNX)
        return "ONX Neutrino";
    #elif defined(Q_OS_RELIANT)
        return "Reliant UNIX";
    #elif defined(Q_OS_SCO)
        return "SCO OpenServer 5";
    #elif defined(Q_OS_SOLARIS)
        return "Oracle Solaris";
    #elif defined(Q_OS_SYMBIAN)
        switch (QSysInfo::SymbianVersion)
        {
            case QSysInfo::SV_SF_4:
                return "Symbian^4";
            case QSysInfo::SV_SF_3:
                return "Symbian^3 (S60 5th Edition Feature Pack 2)";
            case QSysInfo::SV_SF_2:
                return "Symbian^2 (S60 5th Edition Feature Pack 1)";
            case QSysInfo::SV_SF_1:
                return "Symbian^1/Symbian OS v9.4 (S60 5th Edition)";
            case QSysInfo::SV_9_3:
                return "Symbian OS v9.3 (S60 3rd Edition Feature Pack 2)";
            case QSysInfo::SV_9_2:
                return "Symbian OS v9.2 (S60 3rd Edition Feature Pack 1)";
            default:
                return "Symbian";
        }
    #elif defined(Q_OS_ULTRIX)
        return "DEC Ultrix";
    #elif defined(Q_OS_UNIXWARE)
        return "UnixWare 7/Open UNIX 8";
    #else
        return "Unix";
    #endif
#elif defined(Q_OS_MSDOS)
    return "MS-DOS";
#elif defined(Q_OS_OS2)
    return "OS/2";
#elif defined(Q_OS_OS2EMX)
    return "XFree86 on OS/2 (not PM)";
#else
    return "Unknown";
#endif
}

QString PlatformInformation::compilerString()
{
#if defined(Q_CC_BOR)
    return "Borland/Turbo C++";
#elif defined(Q_CC_CDS)
    return "Reliant C++";
#elif defined(Q_CC_COMEAU)
    return "Comeau C++";
#elif defined(Q_CC_DEC)
    return "DEC C++";
#elif defined(Q_CC_EDG)
    return "Edison Design Group C++";
#elif defined(Q_CC_GHS)
    return "Green Hills Optimizing C++ Compilers";
#elif defined(Q_CC_GNU)
    return "GNU C++";
#elif defined(Q_CC_HIGHC)
    return "MetaWare High C/C++";
#elif defined(Q_CC_HPACC)
    return "HP aC++";
#elif defined(Q_CC_INTEL)
    return "Intel C++";
#elif defined(Q_CC_KAI)
    return "KAI C++";
#elif defined(Q_CC_MIPS)
    return "MIPSpro C++";
#elif defined(Q_CC_MSVC)
    return "Microsoft Visual C/C++";
#elif defined(Q_CC_MWERKS)
    return "Metrowerks CodeWarrior";
#elif defined(Q_CC_OC)
    return "CenterLine C++";
#elif defined(Q_CC_PGI)
    return "Portland Group C++";
#elif defined(Q_CC_SUN)
    return "Forte Developer/Sun Studio C++";
#elif defined(Q_CC_SYM)
    return "Digital Mars C/C++";
#elif defined(Q_CC_USLC)
    return "SCO OUDK and UDK";
#elif defined(Q_CC_WAT)
    return "Watcom C++";
#else
    return "Unknown";
#endif
}

/*!
    Gets a string representing the byte order (endianness) of the platform
    on which the application is running.
 */
QString PlatformInformation::byteOrder()
{
    switch (QSysInfo::ByteOrder)
    {
        case QSysInfo::LittleEndian:
            return "little-endian";
        case QSysInfo::BigEndian:
            return "big-endian";
        default:
            return "unknown";
    }
}

/*!
    Gets the size in bits of a pointer for the platform on which the application is compiled (32 or 64).
 */
int PlatformInformation::wordSize()
{
    return QSysInfo::WordSize;
}
