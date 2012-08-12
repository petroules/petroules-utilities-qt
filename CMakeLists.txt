cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include(qt)

project(petroules-utilities-qt)

find_package(Qt4 4.6.0 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
if(QT4_FOUND)
    include(${QT_USE_FILE})
endif()

add_definitions(-Wall)

if(MSVC)
    add_definitions(-DNOMINMAX) # Needed to avoid an issue with std::numeric_limits<T>
endif()

# Path to the source directories of Qt Solutions projects
set(QSA_DIR "${CMAKE_SOURCE_DIR}/lib/qtsolutions/qtsingleapplication/src")
set(QPB_DIR "${CMAKE_SOURCE_DIR}/lib/qtsolutions/qtpropertybrowser/src")

# Set output directories for binaries and static and shared libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src)
add_subdirectory(demos)
