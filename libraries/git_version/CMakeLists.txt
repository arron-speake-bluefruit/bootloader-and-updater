project(git_version)

set(SOURCE_FILE "${PROJECT_BINARY_DIR}/git_version.c")

add_custom_target(git_version_source
    COMMAND ./generate.sh "${SOURCE_FILE}"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    BYPRODUCTS "${SOURCE_FILE}"
)

add_library(${PROJECT_NAME} OBJECT
    "${SOURCE_FILE}"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    "-Wall" "-Wextra" "-Wpedantic"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED On
    C_EXTENSIONS Off
)
