project(linker_script_standalone)

add_library(${PROJECT_NAME} INTERFACE)

set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/STM32F051R8.ld")
target_link_options(${PROJECT_NAME} INTERFACE "-T${LINKER_SCRIPT}")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS "${LINKER_SCRIPT}")
