#include "command_parser.h"
#include <stddef.h>

enum {
    buffer_size = 32,
};

static command_parser_finished_callback_t on_finish;
static command_parser_error_callback_t on_error;

static char buffer[buffer_size];
static size_t index = 0;

void command_parser_initialize(
    command_parser_error_callback_t error_callback,
    command_parser_finished_callback_t finish_callback
) {
    on_finish = finish_callback;
    on_error = error_callback;
    index = 0;
}

static void handle_newline(void) {
    if (index == 0) {
        // Empty inputs produce no output, do nothing.
    } else if (index >= (buffer_size - 1)) {
        // Run out of space to push the null terminator for this input, can't emit it.
        on_error(command_parser_error_too_long);
        index = 0;
    } else {
        buffer[index] = '\0';
        on_finish(buffer);
        index = 0;
    }
}

void command_parser_push(char c) {
    if (c == '\n') {
        handle_newline();
    } else if (index == buffer_size) {
        // Line is full, do nothing.
    } else {
        // More input. For now, everything is treated as a single string.
        buffer[index] = c;
        index += 1;
    }
}

const char* command_parser_error_as_string(command_parser_error_t error) {
    switch (error) {
    case command_parser_error_too_long:
        return "input too long";
    default:
        return "?";
    }
}
