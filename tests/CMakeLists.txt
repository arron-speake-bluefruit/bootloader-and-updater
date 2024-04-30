cmake_minimum_required(VERSION 3.21)
project(tests LANGUAGES C)
enable_testing()

# List of tests to execute

set(tests
    application_command_parser
    application_event_queue
    application_ringbuffer
    application_xmodem_parser
)

# Definitions of test sources & include directories

set(application_command_parser_sources
    "../application/source/command_parser.c"
)
set(application_command_parser_includes
    "../application/include"
)

set(application_event_queue_sources
    "../application/source/event_queue.c"
)
set(application_event_queue_includes
    "../application/include"
)

set(application_ringbuffer_sources
    "../application/source/ringbuffer.c"
)
set(application_ringbuffer_includes
    "../application/include"
)

set(application_xmodem_parser_sources
    "../application/source/xmodem_parser.c"
)
set(application_xmodem_parser_includes
    "../application/include"
)

# Test setup

foreach (test_name ${tests})
    add_executable("${test_name}"
        "tests/${test_name}_tests.c"
        "tests/test_assert.c"
        ${${test_name}_sources}
    )

    target_compile_options("${test_name}" PRIVATE
        "-Wall" "-Wextra" "-Wpedantic"
    )
    target_include_directories("${test_name}" PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
        ${${test_name}_includes}
    )
    set_target_properties("${test_name}" PROPERTIES
        C_STANDARD 17
        C_STANDARD_REQUIRED On
        C_EXTENSIONS Off
    )

    add_test(NAME "${test_name}" COMMAND "${test_name}")
endforeach ()
