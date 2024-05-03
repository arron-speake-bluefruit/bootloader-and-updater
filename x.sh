#!/bin/bash
set -e

# Constants

BUILD_DIR=build
TEST_SOURCE_DIR=tests
TEST_BUILD_DIR=build_tests
OPENOCD_BOARD_SCRIPT=board/stm32f0discovery.cfg

# Subcommand functions

function build_command {
    CMAKE_BUILD_TYPE=Debug
    CLEAN_FIRST=false

    for ARG in "$@"; do
        case "$ARG" in
            -r | --release)
                CMAKE_BUILD_TYPE=Release
                ;;
            -c | --clean)
                CLEAN_FIRST=true
                ;;
            -rc | -cr) # ;]
                CMAKE_BUILD_TYPE=Release
                CLEAN_FIRST=true
                ;;
            *)
                echo "$0 build: unknown argument $ARG" >&2
                exit 1
                ;;
        esac
    done

    if $CLEAN_FIRST; then
        clean_command
    fi

    CMAKE_EXPORT_COMPILE_COMMANDS=On \
        CMAKE_TOOLCHAIN_FILE=toolchain.cmake \
        cmake -S . -B $BUILD_DIR \
        "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE"

    cmake --build $BUILD_DIR --parallel
}

function program_command {
    for ARG in "$@"; do
        case "$ARG" in
            application)
                ELF_PATH="$BUILD_DIR/application/application"
                ;;
            bootloader)
                ELF_PATH="$BUILD_DIR/bootloader/bootloader"
                ;;
            *)
                echo "$0 program: unknown argument $ARG" >&2
                exit 1
        esac
    done

    if [[ "$ELF_PATH" == "" ]]; then
        echo "$0 program: expected 'bootloader' or 'application' argument" >&2
        exit 1
    fi

    openocd \
        -f $OPENOCD_BOARD_SCRIPT \
        -c "program ${ELF_PATH} verify reset" \
        -c "exit"
}

function test_command {
    NORUN=false

    for ARG in "$@"; do
        case "$ARG" in
            -n | --no-run)
                NORUN=true
                ;;
            *)
                echo "$0 test: unknown argument $ARG" >&2
                exit 1
                ;;
        esac
    done

    cmake -S "$TEST_SOURCE_DIR" -B "$TEST_BUILD_DIR" "-DCMAKE_BUILD_TYPE=Debug"
    cmake --build "$TEST_BUILD_DIR" --parallel

    if $NORUN; then
        echo "$0 test: skipping tests"
    else
        ctest --test-dir "$TEST_BUILD_DIR" --output-on-failure
    fi
}

function clean_command {
    rm -rf $BUILD_DIR $TEST_BUILD_DIR
    echo "$0 clean: removed build directories"
}

function flash_command {
    if [ $# -ne 2 ]; then
        echo "$0 flash: expected exactly 2 arguments"
        exit 1
    fi

    openocd -f $OPENOCD_BOARD_SCRIPT -c "program $1 verify reset exit $2"
}

function openocd_command {
    openocd -f $OPENOCD_BOARD_SCRIPT
}

function gdb_command {
    DEBUG_IMAGE_PATH=./build/application/application

    for ARG in "$@"; do
        case "$ARG" in
            application)
                DEBUG_IMAGE_PATH=./build/application/application
                ;;
            bootloader)
                DEBUG_IMAGE_PATH=./build/bootloader/bootloader
                ;;
            *)
                echo "$0 gdb: unknown argument $ARG" >&2
                exit 1
                ;;
        esac
    done

    arm-none-eabi-gdb \
        -ex "file $DEBUG_IMAGE_PATH" \
        -ex "target extended-remote localhost:3333" \
        -ex "break reset_handler" \
        -ex "monitor reset init"
}

function help_command {
    echo "./x.sh"
    echo "Build, debug and test tool for bootloader-and-updater project."
    echo
    echo "USAGE"
    echo "    ./x.sh <subcommand> [...]"
    echo
    echo "SUBCOMMANDS & ARGUMENTS"
    echo "    'b' 'build'     Configure & build the embedded project"
    echo "                    [-r|--release]    release build instead of debug build"
    echo "                    [-c|--clean]      clean before starting the build"
    echo
    echo "    'p' 'program'   Program previously built images onto a board"
    echo "                    <application|bootloader>    select which image to program"
    echo
    echo "    't' 'test'      Configure, build and run the test project"
    echo "                    [-n|--no-run]    don't run tests, just build them"
    echo
    echo "    'c' 'clean'     Remove test & embedded build directories"
    echo
    echo "    'f' 'flash'     Program a binary image directly to a location in flash via OpenOCD"
    echo "                    <filename>    path to file to write to device's flash"
    echo "                    <location>    location in memory to write to (eg. 0x08003000)"
    echo
    echo "    'o' 'openocd'   Start a persistent OpenOCD connection to the device."
    echo
    echo "    'g' 'gdb'       Start an arm-none-eabi-gdb session."
    echo "                    [application|bootloader]    select which image to debug"
    echo "                                                default is 'application'"
    echo
    echo "    'h' 'help'      Display this help message"
}

# Argument parsing

if [ $# -lt 1 ]; then
    echo "$0: expected a subcommand"
    echo
    help_command
    exit 1
fi

SUBCOMMAND="$1"
shift

case $SUBCOMMAND in
    b | build)
        build_command $*
        ;;
    p | program)
        program_command $*
        ;;
    t | test)
        test_command $*
        ;;
    c | clean)
        clean_command $*
        ;;
    f | flash)
        flash_command $*
        ;;
    o | openocd)
        openocd_command $*
        ;;
    g | gdb)
        gdb_command $*
        ;;
    h | help)
        help_command $*
        ;;
    *)
        echo "$0 $SUBCOMMAND: unknown subcommand"
        exit 1
        ;;
esac
