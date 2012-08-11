set(SOURCES
    platformintegrationwindow.cpp
    platformintegrationwindow.ui
    main.cpp
    resources.rc
    resources.qrc
    ../../res/app.icns
)

# Add non-source files to IDE
file(GLOB_RECURSE res "res/*")
list(APPEND SOURCES ${res})

qt_transform_sources(SOURCES "${SOURCES}")

if(APPLE)
    set_source_files_properties(../../res/app.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE app.icns)
endif()

add_executable(platformintegration WIN32 MACOSX_BUNDLE ${SOURCES})
target_link_libraries(platformintegration ${QT_LIBRARIES} petroules-utilities)

if(WIN32)
    target_link_libraries(platformintegration user32)
endif()
