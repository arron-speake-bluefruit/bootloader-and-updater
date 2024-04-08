cmake_minimum_required(VERSION 3.21)
project("bootloader-and-updater" VERSION 0.1.0 LANGUAGES C)

add_subdirectory(drivers/startup)
add_subdirectory(application)
