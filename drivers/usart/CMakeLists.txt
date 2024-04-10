project(usart)

add_library(${PROJECT_NAME} OBJECT
    "${PROJECT_SOURCE_DIR}/usart.c"
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

target_link_libraries(${PROJECT_NAME}
    critical_section
)
