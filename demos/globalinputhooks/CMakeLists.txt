set(SOURCES
    hookmonitorwindow.cpp
    hookmonitorwindow.ui
    main.cpp
    resources.rc
    ../../res/app.icns
)

qt_transform_sources(SOURCES "${SOURCES}")

if(APPLE)
    set_source_files_properties(../../res/app.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE app.icns)
endif()

add_executable(globalinputhooks WIN32 MACOSX_BUNDLE ${SOURCES})
target_link_libraries(globalinputhooks ${QT_LIBRARIES} petroules-utilities)
