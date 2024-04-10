# Drivers

This directory contains all the CMake subproject for low-level Cortex-M0 drivers. These interface
with the core/system peripherals directly.

Most libraries defined here are configured in CMake as _object libraries_ which means they're
compiled individually and linked alongside the dependent applications.

## Critical section

Header-only library providing function-like macros for entering and exiting critical sections.

## GPIO

Low-level interface for the Cortex-M0 General Purpose I/O peripherals.

## RCC

Low-level interface over the Cortex-M0 Reset and Clock Control peripheral.

## Startup

Startup code and linker script for the Cortex-M0, included in executable binaries.

## USART

Low-level interface over the Cortex-M0 USART/UART peripheral.
