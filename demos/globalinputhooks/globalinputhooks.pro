!include(../../petroules-utilities.pri):error(../../petroules-utilities.pri is missing)

# --------------------------------------------------
# This section contains project configuration
# directives such as the required Qt modules, the
# project template, and any variables that need to
# be defined.
# --------------------------------------------------

QT += core gui network
TARGET = globalinputhooks
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
    hookmonitorwindow.h

SOURCES += \
    main.cpp \
    hookmonitorwindow.cpp

FORMS += \
    hookmonitorwindow.ui

OTHER_FILES += \
    resources.rc

# --------------------------------------------------
# This section contains all libraries that the
# project links to, first system libraries, and then
# libraries in the project build tree.
# --------------------------------------------------

macx:LIBS += -framework Cocoa

# Petroules Utilities library
includeLib(../../src, ../../src, petroules-utilities, static)

# --------------------------------------------------
# This section contains miscellaneous commands such
# as Windows resource files and icons for Mac OS X
# --------------------------------------------------

win32:RC_FILE = resources.rc
macx:ICON = ../../res/app.icns
