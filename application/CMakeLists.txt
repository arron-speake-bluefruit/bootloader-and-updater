project(application)

add_executable("${PROJECT_NAME}"
    "${PROJECT_SOURCE_DIR}/source/main.c"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    "-Wall" "-Wextra" "-Wpedantic"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED On
    C_EXTENSIONS Off
)

if(${BUILD_STANDALONE})
    set(APPLICATION_LINKER_PROJECT linker_script_standalone)
else()
    set(APPLICATION_LINKER_PROJECT linker_script_application)
endif()

target_link_libraries(${PROJECT_NAME}
    drivers
    startup
    ${APPLICATION_LINKER_PROJECT}
)
