project(application)

add_executable("${PROJECT_NAME}"
    "${PROJECT_SOURCE_DIR}/source/command_parser.c"
    "${PROJECT_SOURCE_DIR}/source/event_queue.c"
    "${PROJECT_SOURCE_DIR}/source/main.c"
    "${PROJECT_SOURCE_DIR}/source/ringbuffer.c"
    "${PROJECT_SOURCE_DIR}/source/timers.c"
    "${PROJECT_SOURCE_DIR}/source/vector_table.c"
    "${PROJECT_SOURCE_DIR}/source/xmodem_parser.c"
    "${PROJECT_SOURCE_DIR}/source/xmodem.c"
)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")

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
