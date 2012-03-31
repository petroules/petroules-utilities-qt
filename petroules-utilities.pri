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
    lib = $$2
    mode = $$3 # static or shared

    !build_pass:message("Including $$lib from $${path}...")

    win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$${path}/release/ -l$${lib}
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$${path}/debug/ -l$${lib}
    else:symbian: LIBS += -l$${lib}
    else:unix: LIBS += -L$$OUT_PWD/$${path}/ -l$${lib}

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
        win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$${path}/release/$${lib}.lib
        else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$${path}/debug/$${lib}.lib
        else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/$${path}/lib$${lib}.a

        export(PRE_TARGETDEPS)
    }
}
