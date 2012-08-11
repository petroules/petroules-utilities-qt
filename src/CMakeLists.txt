set(TARGET_NAME petroules-utilities)

add_definitions(-DPETROULESUTILITIES_LIBRARY)

set(SOURCES
    dialogs/nativedialogs.cpp
    dialogs/nativefiledialog.cpp
    dialogs/nativeopendirectorydialog.cpp
    dialogs/nativeopenfiledialog.cpp
    dialogs/nativeprintdialog.cpp
    dialogs/nativesavefiledialog.cpp
    dialogs/nativesystemdialog.cpp
    widgets/searchlineedit.cpp
    win32/desktopwindowmanager.cpp
    win32/jumplist.cpp
    win32/thumbbar.cpp
    win32/windowsfileregistration.cpp
    inactivityeventfilter.cpp
    integratedapplication.cpp
    linuxsysteminfo.cpp
    platforminformation.cpp
    qversion.cpp
    systemkeyboardreadwrite.cpp
    translationutils.cpp
    windowmanager.cpp
    integratedmainwindow.cpp
    widgets/explorersearchlineedit.cpp
    nativewindowinfo.cpp
    nativewindow.cpp

    ${QSA_DIR}/qtlocalpeer.cpp
    ${QSA_DIR}/qtsinglecoreapplication.cpp
    ${QSA_DIR}/qtsingleapplication.cpp

    ../res/petroules_utilities_resources.qrc
)

# There's a lot of files in QtPropertyBrowser, add them all
file(GLOB QPB_SRCS ${QPB_DIR}/*.cpp)
list(APPEND SOURCES ${GLOB_QPB})

# Platform-specific sources...
if(WIN32)
    add_definitions(-DWINVER=0x501)

    list(APPEND SOURCES
        win32/explorerstyle.cpp
        windowmanager_win.cpp
        petroules-utilities.rc
    )
elseif(APPLE)
    list(APPEND SOURCES
        mac/cocoainitializer.mm
        integratedapplication_mac.mm
        integratedmainwindow_mac.mm
        windowmanager_mac.mm
        mac/cocoaeventlistener.mm
        mac/icocoaeventreceiver.mm
        mac/machelpers.mm
        mac/macloginitemsmanager.mm
        widgets/searchlineedit_mac.mm
    )
else()
    list(APPEND SOURCES windowmanager_x11.cpp)
endif()

# Add non-source files to IDE
file(GLOB_RECURSE res "../res/*")
list(APPEND SOURCES ${res})

qt_transform_sources(SOURCES "${SOURCES}")

include_directories(.)
include_directories(${QSA_DIR})
include_directories(${QPB_SRC})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} ${QT_LIBRARIES})

if(WIN32)
    target_link_libraries(${TARGET_NAME} comctl32 ole32 gdi32 shlwapi)
    if(MSVC)
        target_link_libraries(${TARGET_NAME} user32 advapi32 shlwapi shell32)
    endif()
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    mark_as_advanced(COCOA_LIBRARY)
    target_link_libraries(${TARGET_NAME} ${QT_LIBRARIES} ${COCOA_LIBRARY})
endif()
