project(startup)

add_library(${PROJECT_NAME} OBJECT
    "${PROJECT_SOURCE_DIR}/source/startup.c"
)

# Configure linker script
set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/source/STM32F051R8.ld")
target_link_options(${PROJECT_NAME} PUBLIC "-T${LINKER_SCRIPT}")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS "${LINKER_SCRIPT}")

target_compile_options(${PROJECT_NAME} PRIVATE
    "-Wall" "-Wextra" "-Wpedantic"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED On
    C_EXTENSIONS Off
)
