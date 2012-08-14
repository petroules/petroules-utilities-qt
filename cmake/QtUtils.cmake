function(qt_transform_sources output_var sources)
    # include output directory for the *_ui.h files
    include_directories(${CMAKE_CURRENT_BINARY_DIR})

    # We use this variable to toggle moc processing on and off
    set(QtUtils_DO_MOC ON)

    # If using Qt, runs rcc, uic, moc and lrelease and populates final_sources
    # with the list of actual source files to compile - if not using
    # Qt we'll simply get all files not ending with .qrc, .ui, .h or .ts
    foreach(file ${sources})
        if("${file}" STREQUAL "<")
            set(QtUtils_DO_MOC OFF)
        elseif("${file}" STREQUAL ">")
            set(QtUtils_DO_MOC ON)
        else()
            list(APPEND final_sources "${file}")
        endif()

        if("${file}" MATCHES "(.*)\\.(rc|manifest|plist|desktop)$")
            source_group("Resources" FILES "${file}")
        elseif("${file}" MATCHES "(.*)\\.(icns|ico)$")
            source_group("Resources\\Icons" FILES "${file}")
        elseif("${file}" MATCHES "(.*)\\.(png|jpg|jpeg|gif|tif|tiff|svg)$")
            source_group("Resources\\Images" FILES "${file}")
        elseif("${file}" MATCHES "(.*)\\.(txt|rtf|pdf)$" AND NOT "${file}" STREQUAL "CMakeLists.txt")
            source_group("Resources\\Other" FILES "${file}")
        elseif(QT4_FOUND AND "${file}" MATCHES "(.*)\\.qrc$")
            qt4_add_resources(outfile "${file}")
            source_group("Qt Resources" FILES "${file}")
        elseif(QT4_FOUND AND "${file}" MATCHES "(.*)\\.ui$")
            qt4_wrap_ui(outfile "${file}")
            source_group("Qt Forms" FILES "${file}")
        elseif(QT4_FOUND AND "${file}" MATCHES "(.*)\\.ts$")
            qt4_add_translation(outfile "${file}")
            source_group("Qt Translations" FILES "${file}")
        elseif(QT4_FOUND AND "${file}" MATCHES "(.*)\\.h$" AND QtUtils_DO_MOC)
            qt4_wrap_cpp(outfile "${file}")
        endif()

        # If we generated anything this round...
        if(outfile)
            list(APPEND final_sources "${outfile}")

            # Don't quote outfile because it's a list containing
            # the same filename twice (odd, I know...)
            source_group("Generated Files" FILES ${outfile})

            if(QtUtils_VERBOSE)
                get_filename_component(outfile_name "${outfile}" NAME)
                message("Generated ${outfile_name}")
            endif()
        endif()
    endforeach()

    # Sets the final_sources var to the parent scope so we can
    # pass it to add_executable or add_library
    set(${output_var} "${final_sources}" PARENT_SCOPE)
endfunction()
