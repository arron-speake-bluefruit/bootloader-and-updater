#!/bin/sh
set -e

BUILD_DIR=build

for ARG in "$@"; do
    case "$ARG" in
        application)
            ELF_PATH="$BUILD_DIR/application/application"
            ;;
        bootloader)
            ELF_PATH="$BUILD_DIR/bootloader/bootloader"
            ;;
        *)
            echo "$0: unknown argument $ARG" >&2
            exit 1
    esac
done

if [[ "$ELF_PATH" == "" ]]; then
    echo "$0: expected 'bootloader' or 'application' argument" >&2
    exit 1
fi

openocd \
    -f board/stm32f0discovery.cfg \
    -c "program ${ELF_PATH} verify reset" \
    -c "exit"
