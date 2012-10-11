cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include(QtUtils)
include(GitUtils)

project(petroules-utilities-qt)

use_qt_sdk_locations()

find_package(Qt4 4.6.0 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
if(QT4_FOUND)
    include(${QT_USE_FILE})
endif()

if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.4)
endif()

add_definitions(-DUNICODE)

if(UNIX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_REL bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_REL lib)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_REL .)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL .)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_REL .)
endif()

set(CPACK_OUTPUT_FILE_PREFIX dist)

# Set output directories for binaries and static and shared libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY_REL})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_LIBRARY_OUTPUT_DIRECTORY_REL})

add_subdirectory(src)
add_subdirectory(demos)
