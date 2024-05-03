cmake_minimum_required(VERSION 3.21)
project("bootloader-and-updater" VERSION 0.1.0 LANGUAGES C)

set(COMMON_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F051R8.ld")

add_subdirectory(application)
add_subdirectory(bootloader)
add_subdirectory(libraries/drivers)
add_subdirectory(libraries/git_version)
add_subdirectory(libraries/linker_script_application)
add_subdirectory(libraries/linker_script_bootloader)
add_subdirectory(libraries/startup)
