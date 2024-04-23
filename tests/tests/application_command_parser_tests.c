#include "test_assert.h"
#include "command_parser.h"
#include <stddef.h>
#include <string.h>

// Mocks

static size_t error_callback_call_count;
static command_parser_error_t error_callback_input;
static void error_callback(command_parser_error_t error) {
    error_callback_input = error;
    error_callback_call_count += 1;
}

static size_t finished_callback_call_count;
static char finished_callback_input[64];
static void finished_callback(const char* input) {
    size_t length = strlen(input);
    ASSERT_LESS_THAN(length, sizeof(finished_callback_input));
    memcpy(finished_callback_input, input, length + 1);

    finished_callback_call_count += 1;
}

// Utility

static void reset_callbacks(void) {
    memset(finished_callback_input, 0x00, sizeof(finished_callback_input));
    finished_callback_call_count = 0;
    error_callback_call_count = 0;
}

// Tests

static void empty_input_produces_no_callback(void) {
    // Given
    reset_callbacks();
    command_parser_initialize(error_callback, finished_callback);

    // When
    command_parser_push('\n');

    // Then
    ASSERT_EQUAL(finished_callback_call_count, 0);
}

static void alphanumeric_input_produces_finished_callback(void) {
    // Given
    reset_callbacks();
    command_parser_initialize(error_callback, finished_callback);

    // When
    command_parser_push('a');
    command_parser_push('b');
    command_parser_push('1');
    command_parser_push('2');
    command_parser_push('3');
    command_parser_push('\n');

    // Then
    ASSERT_EQUAL(finished_callback_call_count, 1);
    ASSERT_STRINGS_EQUAL(finished_callback_input, "ab123");
}

static void very_long_input_produces_error(void) {
    // Given
    reset_callbacks();
    command_parser_initialize(error_callback, finished_callback);

    // When
    for (size_t i = 0; i < 100; i++) {
        command_parser_push('a');
    }

    // Then, error is not produced until newline
    ASSERT_EQUAL(finished_callback_call_count, 0);
    ASSERT_EQUAL(error_callback_call_count, 0);

    // When
    command_parser_push('\n');

    // Then
    ASSERT_EQUAL(finished_callback_call_count, 0);
    ASSERT_EQUAL(error_callback_call_count, 1);
    ASSERT_EQUAL(error_callback_input, command_parser_error_too_long);
}

static void finished_command_are_terminated_with_zero_byte() {
    // Given
    reset_callbacks();
    command_parser_initialize(error_callback, finished_callback);

    // push and discard characters to fill the internal buffer
    for (size_t i = 0; i < 100; i++) {
        command_parser_push('a');
    }
    command_parser_push('\n');
    reset_callbacks();

    // When
    command_parser_push('b');
    command_parser_push('\n');

    // Then
    ASSERT_EQUAL(finished_callback_call_count, 1);
    ASSERT_STRINGS_EQUAL(finished_callback_input, "b");
}

int main(void) {
    empty_input_produces_no_callback();
    alphanumeric_input_produces_finished_callback();
    very_long_input_produces_error();
    finished_command_are_terminated_with_zero_byte();
}
