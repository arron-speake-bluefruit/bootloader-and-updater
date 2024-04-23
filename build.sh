#!/bin/sh
set -e

BUILD_DIR=build

CMAKE_BUILD_TYPE=Debug

for ARG in "$@"; do
    case "$ARG" in
        --release)
            CMAKE_BUILD_TYPE=Release
            ;;
        *)
            echo "$0: unknown argument $ARG" >&2
            exit 1
            ;;
    esac
done

CMAKE_EXPORT_COMPILE_COMMANDS=On \
    CMAKE_TOOLCHAIN_FILE=toolchain.cmake \
    cmake -S . -B $BUILD_DIR \
    "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE"

cmake --build $BUILD_DIR --parallel
