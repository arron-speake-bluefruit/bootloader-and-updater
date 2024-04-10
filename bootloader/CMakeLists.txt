project(bootloader)

add_executable("${PROJECT_NAME}"
    "${PROJECT_SOURCE_DIR}/source/main.c"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    "-Wall" "-Wextra" "-Wpedantic"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED On
    C_EXTENSIONS Off
)

target_link_libraries(${PROJECT_NAME}
    drivers
    startup
    linker_script_bootloader
)
