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

### Installation instructions for Windows

Install the MSYS2 build environment (https://www.msys2.org/), following the instruction on the website.

> If you're already using MinGW-w64, you can use that instead. Just drop the `mingw-w64-x86_64-`
> prefix when installing packages.

Launch the MSYS2 MINGW terminal, run `pacman -S cmake git mingw-w64-x86_64-arm-none-eabi-gcc ninja`
to install CMake, Git, the Arm GCC toolchain and Ninja.

> On Windows, Ninja is used over Make because it handles Windows-style paths better.

For building tests, run `pacman -S gcc` to install native GCC.

For programming and debugging, you can install OpenOCD and GDB by running
`pacman -S mingw-w64-x86_64-openocd mingw-w64-x86_64-arm-none-eabi-gdb`. You'll also need to
install the ST-LINK Drivers for Windows, which are provided by ST at
`https://www.st.com/en/development-tools/stsw-link009.html`. (MyST account required for download).

With the installed requirements, you should be able to use the rest of the readme like normal.

## Building

To build the program run `./x.sh build`. Do `./x.sh help` for more information.

In VSCode, you should be able to use the CMake Tools extension to configure CMake by running the
`CMake: Configure` command. To build, do `F7` or run `CMake: Build`. This should produce a
debug build.

Building produces ELF and binary artifacts in the build directory. You should be able to use
either build method interchangeably.

## Programming

The `./x.sh program` command accepts either `bootloader` or `application` as arguments, and
programs them directly to their proper addresses using OpenOCD.

For example, to program a full release image:
```sh
./x.sh build --release
./x.sh program application
./x.sh program bootloader
```

A binary image can be programmed to the update image region using an x.sh command. For example,
to write the application binary to 0x08003000 (12KiB offset into flash), do:
```sh
./x.sh flash build/application/application.bin 0x08003000
```

## Debugging

Debugging can be performed in the terminal using GDB, or graphically with VSCode.

Debug setup is for an STM32F0DISCOVERY board, via its ST-LINK USB port.

### Debugging with GDB

After building and programming a new image, do `./x.sh openocd` to connect
OpenOCD to the board.

Then, run `./x.sh gdb` to attach the debugger. To debug the bootloader, do `./x.sh gdb bootloader`

Debugging with GDB supports debugging both application and bootloader.

### Debugging with VSCode

The launch tasks 'Debug Application' and 'Debug Bootloader' are provided. Note that there do not
trigger a build before starting the debug session.

Debugging in VSCode requires the recommended `cortex-debug` extension.

## Testing

A single CMake build directory is unable to handle multiple compiler toolchains at once, so the
test project is contained inside its own CMake project with its own build directory.

To build and run tests, do `./x.sh test`. Passing the `--no-run` flag will only perform the build
step.
