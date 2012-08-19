#!/bin/sh

# Clone all submodules
git submodule update --init --recursive

# Make build directories for qmake, and CMake in both shared and static mode
mkdir ../build-qmake ../build-cmake-shared ../build-cmake-static

# Configure using qmake
cd ../build-qmake
qmake ../petroules-utilities-qt/petroules-utilities-qt.pro -r

# Configure using CMake (shared)
cd ../build-cmake-shared
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON ../petroules-utilities-qt

# Configure using CMake (static)
cd ../build-cmake-static
cmake -DCMAKE_BUILD_TYPE=Release ../petroules-utilities-qt

