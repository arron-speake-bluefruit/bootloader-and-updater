cmake_minimum_required(VERSION 3.21)
project("bootloader-and-updater" VERSION 0.1.0 LANGUAGES C)

# Common subproject properties
set(COMMON_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F051R8.ld")
add_compile_options("-Wall" "-Wextra" "-Wpedantic")
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS Off)

add_subdirectory(application)
add_subdirectory(bootloader)
add_subdirectory(libraries/critical_section)
add_subdirectory(libraries/drivers)
add_subdirectory(libraries/external_flash)
add_subdirectory(libraries/flash_copy)
add_subdirectory(libraries/git_version)
add_subdirectory(libraries/info_region)
add_subdirectory(libraries/startup)
