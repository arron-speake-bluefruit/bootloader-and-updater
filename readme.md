# Bootloader and Updater

Cortex-M0 project for demonstrating a bootloader and firmware updater combo.

## Requirements

To build output artifacts:

- CMake (version 3.21 or greater)
- arm-none-eabi-gcc and toolchain (tested with version 13.2.0)
- bash (optional, for running scripts)

To program and debug compiled artifacts:

- OpenOCD (tested with version 0.12.0)
- GDB (optional, part of arm-none-eabi toolchain or gdb-multiarch, for TUI debugging)
- vscode (optional, for graphical debugging)

## Building

To build the program run `./build.sh`. The scripts accepts the flags:
- `--release` to enable release optimizations (`-Os -DNDEBUG`), without this a debug build is made.
- `--standalone` to make the application binary suitable for debug without a bootloader.

In VSCode, you should be able to use the CMake Tools extension to configure CMake by running the
`CMake: Configure` command. To build, do `F7` or run `CMake: Build`. This should produce a debug
standalone build suitable for debugging inside VSCode.

This produces library and executable artifacts in the build directory. You should be able to use
either build method interchangeably.

## Programming

The `./program.sh` script accepts either `bootloader` or `application` as arguments, and programs
them directly to their proper addresses using OpenOCD.

For example, to program a full release image:
```sh
./build.sh --release
./program.sh application
./program.sh bootloader
```
Or a standalone debug application:
```sh
./build.sh --standalone
./program.sh application
```

## Debugging

Debugging can be performed in the terminal using GDB, or graphically with VSCode.

Debug setup is for an STM32F0DISCOVERY board, via its ST-LINK USB port.

### Debugging with GDB

After building and programming a new image, do `openocd -f board/stm32f0discovery.cfg` to connect
OpenOCD to the board.

Then, run `arm-none-eabi-gdb -x debug/gdb.cfg` to attach the debugger. To debug the bootloader,
do `arm-none-eabi-gdb -x debug/gdb-bootloader.cfg`

Debugging with GDB supports debugging standalone, application and bootloader.

### Debugging with VSCode

Using the recommended `cortex-debug` extension, use the "Debug Application" launch task (or `F5`)
to begin a debug session. This should run the built standalone debug application.

There is no current setup for debugging the bootloader inside VSCode.
