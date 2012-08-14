# Path to the source directories of Qt Solutions projects
set(QSA_DIR "../lib/qtsolutions/qtsingleapplication/src")
set(QPB_DIR "../lib/qtsolutions/qtpropertybrowser/src")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${QSA_DIR}"
    "${QPB_DIR}"
)

if(MSVC)
    add_definitions(-DNOMINMAX) # Needed to avoid an issue with std::numeric_limits<T>
endif()

add_definitions(-Wall -DPETROULESUTILITIES_LIBRARY)

set(TARGET_NAME petroules-utilities)

set(SOURCES
    dialogs/nativedialogs.cpp
    dialogs/nativefiledialog.cpp
    dialogs/nativefiledialog.h
    dialogs/nativeopendirectorydialog.cpp
    dialogs/nativeopendirectorydialog.h
    dialogs/nativeopenfiledialog.cpp
    dialogs/nativeopenfiledialog.h
    dialogs/nativeprintdialog.cpp
    dialogs/nativeprintdialog.h
    dialogs/nativesavefiledialog.cpp
    dialogs/nativesavefiledialog.h
    dialogs/nativesystemdialog.cpp
    dialogs/nativesystemdialog.h

    widgets/explorersearchlineedit.cpp
    widgets/explorersearchlineedit.h
    widgets/searchlineedit.cpp
    widgets/searchlineedit.h

    win32/desktopwindowmanager.cpp
    win32/jumplist.cpp
    win32/thumbbar.cpp
    win32/thumbbar.h
    win32/windowsfileregistration.cpp

    inactivityeventfilter.cpp
    inactivityeventfilter.h
    integratedapplication.cpp
    integratedapplication.h
    integratedmainwindow.cpp
    integratedmainwindow.h
    linuxsysteminfo.cpp
    nativewindow.cpp
    nativewindowinfo.cpp
    nativewindowinfo.h
    platforminformation.cpp
    platforminformation.h
    qversion.cpp
    qversion.h
    systemkeyboardreadwrite.cpp
    systemkeyboardreadwrite.h
    translationutils.cpp
    windowmanager.cpp

    ../res/petroules_utilities_resources.qrc

    ${QSA_DIR}/qtlocalpeer.cpp
    ${QSA_DIR}/qtlocalpeer.h
    ${QSA_DIR}/qtsinglecoreapplication.cpp
    ${QSA_DIR}/qtsinglecoreapplication.h
    ${QSA_DIR}/qtsingleapplication.cpp
    ${QSA_DIR}/qtsingleapplication.h
)

# There's a lot of files in QtPropertyBrowser, add them all
file(GLOB QPB_SRCS ${QPB_DIR}/*.cpp)
list(APPEND SOURCES ${GLOB_QPB})

# Platform-specific sources...
if(WIN32)
    add_definitions(-DWINVER=0x501)

    list(APPEND SOURCES
        win32/explorerstyle.cpp
        petroules-utilities.rc
        windowmanager_win.cpp
    )
elseif(APPLE)
    list(APPEND SOURCES
        mac/cocoaeventlistener.mm
        mac/cocoainitializer.mm
        mac/icocoaeventreceiver.mm
        mac/machelpers.mm
        mac/macloginitemsmanager.mm
        widgets/searchlineedit_mac.mm
        integratedapplication_mac.mm
        integratedmainwindow_mac.mm
        windowmanager_mac.mm
    )
else()
    list(APPEND SOURCES windowmanager_x11.cpp)
endif()

# Add non-source files to IDE
file(GLOB_RECURSE res "../res/*")
list(APPEND SOURCES ${res})

qt_transform_sources(SOURCES "${SOURCES}")

add_library("${TARGET_NAME}" ${SOURCES})
target_link_libraries("${TARGET_NAME}" ${QT_LIBRARIES})

if(WIN32)
    target_link_libraries("${TARGET_NAME}" comctl32 ole32 gdi32 shlwapi)
    if(MSVC)
        target_link_libraries("${TARGET_NAME}" user32 advapi32 shlwapi shell32)
    endif()
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    mark_as_advanced(COCOA_LIBRARY)

    target_link_libraries("${TARGET_NAME}" ${COCOA_LIBRARY})
endif()
