# Project setup

Set up:
- CMSIS-driven application
- CMake build system
- Program & debug (gdb & vscode) support

# Event loop

Set up a minimal busy-waiting main event loop, with code support for adding new event types.

# USART Command line

Set up a USART-driven command line interface.

# Update-related commands

Add commands for:
- starting and update
- sending firmware images/image portions
- triggering update

# Application update controller

Module for receiving 'update packet' events over the event loop.

# Bootloader flash driver

Bootloader-side flash driver. Kept separate and simpler from the application flash driver.

# Checksumm driver

If there's hardware support, configure using that for checksumming memory regions.
Otherwise, use software checksum.

# 'update ready' flag

Application settable, bootloader resettable flag for performing an update on reset.

# Bootloader

Modify build setup to:
- Produce minimal (compile-time configurable) bootloader.
- Set base address of application to a fixed offset (boot address).

Add a bootloader binary which immediately resets onto the application image.

# Updating bootloader

Modify the bootloader to:
- check if a firmware update is planned.
- if so:
  - enable flash & checksum drivers
  - checksum validation for update image
  - copy update image onto application image
  - checksum validation for application image
- boot into application

# Bootloader report

Somehow, report the action taken by the bootloader to the application. One of:
- typical - normal boot process
- updated - successfully updated the application image
- bad checksum - acknowledged update request, refused because of bad image checksum
