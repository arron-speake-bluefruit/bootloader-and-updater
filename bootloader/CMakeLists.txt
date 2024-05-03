project(bootloader)

add_executable("${PROJECT_NAME}"
    "${PROJECT_SOURCE_DIR}/source/boot.c"
    "${PROJECT_SOURCE_DIR}/source/main.c"
)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME}
    drivers
    startup
    linker_script_bootloader
)

add_custom_target("${PROJECT_NAME}.bin" ALL
    COMMAND ${CMAKE_OBJCOPY} -Obinary "${PROJECT_NAME}" "${PROJECT_NAME}.bin"
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)
