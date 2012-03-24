# --------------------------------------------------
# This section contains project configuration
# directives such as the required Qt modules, the
# project template, and any variables that need to
# be defined.
# --------------------------------------------------

macx:CONFIG += staticlib
TEMPLATE = lib
DEFINES += PETROULESUTILITIES_LIBRARY
win32-msvc*:DEFINES += NOMINMAX # Needed to avoid an issue with std::numeric_limits<T>
TARGET = petroules-utilities

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

PRECOMPILED_HEADER += stable.h

HEADERS += \
    petroules-utilities-global.h \
    dialogs/nativedialogs.h \
    dialogs/nativefiledialog.h \
    dialogs/nativeopendirectorydialog.h \
    dialogs/nativeopenfiledialog.h \
    dialogs/nativeprintdialog.h \
    dialogs/nativesavefiledialog.h \
    dialogs/nativesystemdialog.h \
    widgets/searchlineedit.h \
    win32/desktopwindowmanager.h \
    win32/jumplist.h \
    win32/jumplists_win32.h \
    win32/thumbbar.h \
    win32/windowsfileregistration.h \
    win32/windowstaskbarintegration_includes.h \
    inactivityeventfilter.h \
    integratedapplication.h \
    linuxsysteminfo.h \
    platforminformation.h \
    qversion.h \
    stable.h \
    petroules-utilities.h \
    systemkeyboardreadwrite.h \
    translationutils.h \
    version.h \
    windowmanager.h \
    integratedapplication_p.h \
    integratedmainwindow_p.h \
    integratedmainwindow.h \
    widgets/explorersearchlineedit.h

SOURCES += \
    dialogs/nativedialogs.cpp \
    dialogs/nativefiledialog.cpp \
    dialogs/nativeopendirectorydialog.cpp \
    dialogs/nativeopenfiledialog.cpp \
    dialogs/nativeprintdialog.cpp \
    dialogs/nativesavefiledialog.cpp \
    dialogs/nativesystemdialog.cpp \
    widgets/searchlineedit.cpp \
    win32/desktopwindowmanager.cpp \
    win32/jumplist.cpp \
    win32/thumbbar.cpp \
    win32/windowsfileregistration.cpp \
    inactivityeventfilter.cpp \
    integratedapplication.cpp \
    linuxsysteminfo.cpp \
    platforminformation.cpp \
    qversion.cpp \
    systemkeyboardreadwrite.cpp \
    translationutils.cpp \
    windowmanager.cpp \
    integratedmainwindow.cpp \
    widgets/explorersearchlineedit.cpp

RESOURCES += \
    ../res/petroules_utilities_resources.qrc

OTHER_FILES += \
    petroules-utilities.rc

# --------------------------------------------------
# This section contains all libraries that the
# project links to, first system libraries, and then
# libraries in the project build tree.
# --------------------------------------------------

win32 {
    DEFINES += WINVER=0x501
    LIBS += -lcomctl32 -lole32 -lgdi32 -lshlwapi
    win32-msvc* {
        LIBS += -luser32 -ladvapi32 -lshlwapi -lshell32
    }

    HEADERS += \
        win32/explorerstyle.h

    SOURCES += \
        win32/explorerstyle.cpp \
        windowmanager_win.cpp
}

macx {
    HEADERS += \
        mac/cocoainitializer.h \
        windowmanager_mac_p.h \
        mac/icocoaeventreceiver.h \
        mac/macloginitemsmanager.h

    OBJECTIVE_HEADERS += \
        mac/cocoaeventlistener.h \
        mac/machelpers.h

    OBJECTIVE_SOURCES += \
        mac/cocoainitializer.mm \
        integratedapplication_mac.mm \
        integratedmainwindow_mac.mm \
        windowmanager_mac.mm \
        mac/cocoaeventlistener.mm \
        mac/icocoaeventreceiver.mm \
        mac/machelpers.mm \
        mac/macloginitemsmanager.mm \
        widgets/searchlineedit_mac.mm

    LIBS += -framework Cocoa # ApplicationServices & CoreFoundation
}

unix:!macx {
    SOURCES += windowmanager_x11.cpp
}

win32|macx|unix {
    !include(../lib/qtsingleapplication/src/qtsingleapplication.pri) {
        error(Could not find the qtsingleapplication.pri file!)
    }
}

# --------------------------------------------------
# This section contains miscellaneous commands such
# as Windows resource files and icons for Mac OS X
# --------------------------------------------------

win32:RC_FILE = petroules-utilities.rc

# --------------------------------------------------
# This section contains commands for deployment to
# various platforms, especially mobile devices
# --------------------------------------------------

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3A63D34
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = petroules-utilities.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
