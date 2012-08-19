set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources.rc
    ../../res/app.icns
)

qt_transform_sources(SOURCES "${SOURCES}")

if(BUILD_SHARED_LIBS)
    add_definitions(-DPETROULESUTILITIES_IMPORTS)
endif()

if(APPLE)
    set_source_files_properties(../../res/app.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE app.icns)
endif()

add_executable(windowsystem WIN32 MACOSX_BUNDLE ${SOURCES})
target_link_libraries(windowsystem ${QT_LIBRARIES} petroules-utilities)

if(WIN32)
    target_link_libraries(windowsystem user32 dwmapi ${QT_QTMAIN_LIBRARY})
endif()
