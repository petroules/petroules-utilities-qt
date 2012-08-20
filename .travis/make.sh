#!/bin/sh
set -e

# Build all variants
cd ..
make -C build-qmake
make -C build-cmake-shared
make -C build-cmake-static
