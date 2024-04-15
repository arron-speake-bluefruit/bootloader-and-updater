#!/bin/sh
set -e

SOURCE_DIR=tests
BUILD_DIR=build_tests
NORUN=false

for ARG in "$@"; do
    case "$ARG" in
        --no-run)
            NORUN=true
            ;;
        *)
            echo "$0: unknown argument $ARG" >&2
            exit 1
            ;;
    esac
done

cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" "-DCMAKE_BUILD_TYPE=Debug"
cmake --build "$BUILD_DIR" --parallel

if $NORUN; then
    echo "$0: skipping tests"
else
    ctest --test-dir "$BUILD_DIR" --output-on-failure
fi
