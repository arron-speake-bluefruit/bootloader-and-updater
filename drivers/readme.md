# Drivers

This directory contains all the CMake subproject for low-level Cortex-M0 drivers. These interface
with the core/system peripherals directly.

The libraries defined here are configured in CMake as _object libraries_ which means they're
compiled individually and linked alongside the dependent applications.
