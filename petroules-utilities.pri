!isEmpty(PETROULES_PRI_INCLUDED):error("petroules.pri already included")
PETROULES_PRI_INCLUDED = 1

# Shh... secret and private :)
win32:_PETROULES_UTILITIES_LIBS_ += -lcomctl32 -lole32 -lgdi32 -lshlwapi
win32-msvc*:_PETROULES_UTILITIES_LIBS_ += -luser32 -ladvapi32 -lshlwapi -lshell32
macx:_PETROULES_UTILITIES_LIBS_ += -framework Cocoa

# Add missing X11 scope
!win32:!embedded:!qpa:!mac:!symbian:CONFIG += x11

defineReplace(formatpath) {
    path = $$1

    win32 {
        return(\"$$replace(path, "/", "\\")\")
    } else {
        return($$replace(path, " ", "\\ "))
    }
}

# Prints dependency information
defineTest(printDependencyInfo) {
    !build_pass:message("CURRENT DEPENDENCY CONFIGURATION:")
    !build_pass:message("LIBS: $$LIBS")
    !build_pass:message("INCLUDEPATH: $$INCLUDEPATH")
    !build_pass:message("DEPENDPATH: $$DEPENDPATH")
    !build_pass:message("PRE_TARGETDEPS: $$PRE_TARGETDEPS")
}

# Includes an arbitrary library
defineTest(includeLib) {
    path = $$1
    libdir = $$2
    lib = $$3
    mode = $$4 # static or shared

    !build_pass:message("Including $$lib from $${path}...")

    win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$${libdir}/release/ -l$${lib}
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$${libdir}/debug/ -l$${lib}
    else:symbian: LIBS += -l$${libdir}
    else:unix: LIBS += -L$$OUT_PWD/$${libdir}/ -l$${lib}

    INCLUDEPATH += $$PWD/$${path}
    DEPENDPATH += $$PWD/$${path}

    isEqual(lib, petroules-utilities) {
        !build_pass:message("Including extra headers for QtSolutions...")

        isEqual(mode, static) {
            DEFINES += PETROULESUTILITIES_STATIC
            export(DEFINES)
            win32:LIBS += $$_PETROULES_UTILITIES_LIBS_
            win32:export(LIBS)
        }

        # We have to make sure we include these headers' paths
        # because they will get indirectly included from THIS project
        QTSOLUTIONS_PATH = $$1/../lib/qtsolutions/qtsingleapplication/src
        INCLUDEPATH += $$PWD/$$QTSOLUTIONS_PATH
        DEPENDPATH += $$PWD/$$QTSOLUTIONS_PATH

        QTPROPERTYBROWSER_PATH = $$1/../lib/qtsolutions/qtpropertybrowser/src
        INCLUDEPATH += $$PWD/$$QTPROPERTYBROWSER_PATH
        DEPENDPATH += $$PWD/$$QTPROPERTYBROWSER_PATH
    }

    export(LIBS)
    export(INCLUDEPATH)
    export(DEPENDPATH)

    # This is only for statically linked libraries
    isEqual(mode, static) {
        win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$${libdir}/release/$${lib}.lib
        else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$${libdir}/debug/$${lib}.lib
        else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/$${libdir}/lib$${lib}.a

        export(PRE_TARGETDEPS)
    }
}

# Copies the given files to the destination directory
defineTest(copyToDestdir) {
    files = $$1

    for(FILE, files) {
        DDIR = $$DESTDIR/$$2

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        # Make sure the destination directory exists
        QMAKE_POST_LINK += $$QMAKE_MKDIR $$quote($$DDIR) $$escape_expand(\\n\\t)

        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

defineTest(copyDirToDestdir) {
    dir = $$1
    DDIR = $$DESTDIR

    # Replace slashes in paths with backslashes for Windows
    win32:dir ~= s,/,\\,g
    win32:DDIR ~= s,/,\\,g

    unix:QMAKE_COPY_RECURSIVE = $$QMAKE_COPY -R

    QMAKE_POST_LINK += $$QMAKE_COPY_RECURSIVE $$quote($$dir) $$quote($$DDIR) $$escape_expand(\\n\\t)

    export(QMAKE_POST_LINK)
}
