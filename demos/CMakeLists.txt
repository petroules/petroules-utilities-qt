# Path to the source directories of Qt Solutions projects
set(QSA_DIR "../lib/qtsolutions/qtsingleapplication/src")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_SOURCE_DIR}/src"
    "${QSA_DIR}"
)

if(MSVC)
    add_definitions(-DNOMINMAX) # Needed to avoid an issue with std::numeric_limits<T>
endif()

add_subdirectory(globalinputhooks)
add_subdirectory(platformintegration)
add_subdirectory(windowsystem)
