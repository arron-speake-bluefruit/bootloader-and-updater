# Drivers

This directory contains all the CMake subproject for low-level Cortex-M0 drivers. These interface
with the core/system peripherals directly.

Most libraries defined here are configured in CMake as _object libraries_ which means they're
compiled individually and linked alongside the dependent applications.

## Critical section

Header-only library providing function-like macros for entering and exiting critical sections.

## GPIO

Low-level interface for the Cortex-M0 General Purpose I/O peripherals.

## Linker scripts

Included by executable binaries for configuring the link process. These are
- `application`, for linking application code to be jumped into by the bootloader;
- `bootloader`, for linking the bootloader itself; and
- `standalone`, for linking the application to run without a bootloader.

## RCC

Low-level interface over the Cortex-M0 Reset and Clock Control peripheral.

## SCB

Low-level interface for the Cortex-M0 System Control Block core peripheral.

## Startup

Startup code for the Cortex-M0, included in executable binaries.

## Syscfg

Low-level interface for the Cortex-M0 System Configuration peripheral.

## USART

Low-level interface over the Cortex-M0 USART/UART peripheral.
