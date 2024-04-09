#!/bin/sh
set -e

BUILD_DIR=build

if [[ "$1" = "--release" ]]; then
    CMAKE_BUILD_TYPE=Release
else
    CMAKE_BUILD_TYPE=Debug
fi

CMAKE_EXPORT_COMPILE_COMMANDS=On \
    CMAKE_TOOLCHAIN_FILE=toolchain.cmake \
    cmake -S . -B $BUILD_DIR "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE"

cmake --build $BUILD_DIR --parallel
