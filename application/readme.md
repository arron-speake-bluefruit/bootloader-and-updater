# Application

This is the main application code for the project.

In standalone builds, this links using `drivers/linker_script_standalone` and can be flashed and
debugged directly. Normally, it links using `drivers/linker_script_application` and must be flashed
into the expected address for the bootloader to jump into.
