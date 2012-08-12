# Path to the source directories of Qt Solutions projects
set(QSA_DIR "../lib/qtsolutions/qtsingleapplication/src")
set(QPB_DIR "../lib/qtsolutions/qtpropertybrowser/src")

include_directories(
    .
    ../src
    ${QSA_DIR}
    ${QPB_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

if(MSVC)
    add_definitions(-DNOMINMAX) # Needed to avoid an issue with std::numeric_limits<T>
endif()

add_definitions(-Wall)

add_subdirectory(globalinputhooks)
add_subdirectory(platformintegration)
add_subdirectory(windowsystem)
