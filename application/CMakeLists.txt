project(application)

add_executable("${PROJECT_NAME}"
    "${PROJECT_SOURCE_DIR}/source/command_parser.c"
    "${PROJECT_SOURCE_DIR}/source/event_queue.c"
    "${PROJECT_SOURCE_DIR}/source/main.c"
    "${PROJECT_SOURCE_DIR}/source/ringbuffer.c"
    "${PROJECT_SOURCE_DIR}/source/vector_table.c"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    "-Wall" "-Wextra" "-Wpedantic"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED On
    C_EXTENSIONS Off
)

target_link_libraries(${PROJECT_NAME}
    drivers
    git_version
    linker_script_application
    startup
)

add_custom_target("${PROJECT_NAME}.bin" ALL
    COMMAND ${CMAKE_OBJCOPY} -Obinary "${PROJECT_NAME}" "${PROJECT_NAME}.bin"
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
