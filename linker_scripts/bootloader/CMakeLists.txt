project(linker_script_bootloader)

add_library(${PROJECT_NAME} INTERFACE)
target_link_options(${PROJECT_NAME} INTERFACE
    "-T${PROJECT_SOURCE_DIR}/STM32F051R8.ld"
    "-T${PROJECT_SOURCE_DIR}/../common.ld"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERFACE_LINK_DEPENDS "${PROJECT_SOURCE_DIR}/STM32F051R8.ld;${PROJECT_SOURCE_DIR}/../common.ld"
)
