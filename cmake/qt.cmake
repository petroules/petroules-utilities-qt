function(qt_transform_sources output_var sources)
    # include output directory for the *_ui.h files
    include_directories(${CMAKE_CURRENT_BINARY_DIR})

    # If using Qt, runs rcc, uic and lrelease and populates final_sources
    # with the list of actual source files to compile - if not using
    # Qt we'll simply get all files not ending with .qrc, .ui or .ts
    foreach(file ${sources})
        if(QT4_FOUND AND ${file} MATCHES "^(.*).qrc$")
            qt4_add_resources(outfile ${file}) # rcc
            get_filename_component(outfile_name "${outfile}" NAME)
            message("Generating ${outfile_name}")
        elseif(QT4_FOUND AND ${file} MATCHES "^(.*).ui$")
            qt4_wrap_ui(outfile ${file}) # uic
            get_filename_component(outfile_name "${outfile}" NAME)
            message("Generating ${outfile_name}")
        elseif(QT4_FOUND AND ${file} MATCHES "^(.*).ts$")
            qt4_add_translation(outfile ${file})
            get_filename_component(outfile_name "${outfile}" NAME)
            message("Generating ${outfile_name}")
        else()
            set(outfile "${file}")
        endif()

        list(APPEND final_sources "${outfile}")
    endforeach()

    # Sets the final_sources var to the parent scope so we can
    # pass it to add_executable or add_library
    set(${output_var} "${final_sources}" PARENT_SCOPE)
endfunction()
