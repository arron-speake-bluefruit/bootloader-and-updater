cmake_minimum_required(VERSION 3.21)
project("bootloader-and-updater" VERSION 0.1.0 LANGUAGES C)

option(BUILD_STANDALONE "Build the application using standalone startup code." Off)

add_subdirectory(application)
add_subdirectory(bootloader)
add_subdirectory(drivers)
add_subdirectory(startup)
add_subdirectory(linker_scripts/application)
add_subdirectory(linker_scripts/bootloader)
add_subdirectory(linker_scripts/standalone)
