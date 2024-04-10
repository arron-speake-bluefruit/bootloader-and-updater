project(startup)

add_library(${PROJECT_NAME} OBJECT
    "${PROJECT_SOURCE_DIR}/startup.c"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    "-Wall" "-Wextra" "-Wpedantic"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED On
    C_EXTENSIONS Off
)
