# --------------------------------------------------
# This section contains project configuration
# directives such as the required Qt modules, the
# project template, and any variables that need to
# be defined.
# --------------------------------------------------

QT += core gui network
TARGET = platformintegration
win32-msvc*:DEFINES += NOMINMAX # Needed to avoid an issue with std::numeric_limits<T>
TEMPLATE = app

# --------------------------------------------------
# This section contains all the main code/resource
# files that make up the target, including:
# * precompiled header (.pch)
# * headers (.h)
# * sources (.c, .cpp, .m, .mm, ...)
# * forms (.ui)
# * resources (.qrc)
# * translations (.tr)
# * any other files belonging in the OTHER_FILES var
# --------------------------------------------------

HEADERS += \
    mainwindow.h

SOURCES += \
    main.cpp \
    mainwindow.cpp

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    resources.rc

# --------------------------------------------------
# This section contains all libraries that the
# project links to, first system libraries, and then
# libraries in the project build tree.
# --------------------------------------------------

macx:LIBS += -framework Cocoa

PETROULESUTILITIES_PATH = ../../src
QTSOLUTIONS_PATH = $$PETROULESUTILITIES_PATH/../lib/qtsingleapplication/src

# Petroules Utilities library

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/$$PETROULESUTILITIES_PATH/release/ -lpetroules-utilities
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/$$PETROULESUTILITIES_PATH/debug/ -lpetroules-utilities
else:symbian: LIBS += -lpetroules-utilities
else:unix: LIBS += -L$$OUT_PWD/$$PETROULESUTILITIES_PATH/ -lpetroules-utilities

# We have to make sure we include the QtSingleApplication headers
# path because it will get indirectly included from THIS project
INCLUDEPATH += $$PWD/$$PETROULESUTILITIES_PATH $$PWD/$$QTSOLUTIONS_PATH
DEPENDPATH += $$PWD/$$PETROULESUTILITIES_PATH $$PWD/$$QTSOLUTIONS_PATH

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$PETROULESUTILITIES_PATH/release/petroules-utilities.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/$$PETROULESUTILITIES_PATH/debug/petroules-utilities.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/$$PETROULESUTILITIES_PATH/libpetroules-utilities.a

# --------------------------------------------------
# This section contains miscellaneous commands such
# as Windows resource files and icons for Mac OS X
# --------------------------------------------------

win32:RC_FILE = resources.rc
macx:ICON = ../../res/app.icns
