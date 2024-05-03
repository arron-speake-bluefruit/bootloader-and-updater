project(git_version)

set(SOURCE_FILE "${PROJECT_BINARY_DIR}/git_version.c")

add_custom_target(git_version_source
    COMMAND ./generate.sh "${SOURCE_FILE}"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    BYPRODUCTS "${SOURCE_FILE}"
)

add_library(${PROJECT_NAME} OBJECT "${SOURCE_FILE}")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
