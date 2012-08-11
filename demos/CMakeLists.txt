include_directories(.)
include_directories(../src)
include_directories(${QSA_DIR})
include_directories(${QPB_SRC})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(globalinputhooks)
add_subdirectory(platformintegration)
add_subdirectory(windowsystem)
