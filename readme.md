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

To perform a debug build, run `./build.sh`. A release build is performed by `./build.sh --release`.
The build script handles both CMake configuration and building.

In VSCode, you should be able to use the CMake Tools extension to configure CMake by running the
`CMake: Configure` command. To build, do `F7` or run `CMake: Build`.

This produces library and executable artifacts in the build directory. You should be able to use
either build method interchangeably.

## Debugging

Debugging can be performed in the terminal using GDB, or graphically with VSCode.

Debug setup is for an STM32F0DISCOVERY board, via its ST-LINK USB port.

### Debugging with GDB

In the project root directory, run `openocd -f debug/openocd.cfg` to start OpenOCD. This connects
to the board and programs the application binary.

Then, run `arm-none-eabi-gdb -x debug/gdb.cfg` to attach the debugger.

### Debugging with vscode

Using the recommended `cortex-debug` extension, use the "Debug Application" launch task to begin a
debug session.

TODO: VSCode debugging might be broken with build-configurable linker scripts now. Check!
