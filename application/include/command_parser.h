#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

typedef enum command_parser_error {
    command_parser_error_too_long,
} command_parser_error_t;

typedef void (*command_parser_error_callback_t)(command_parser_error_t);
typedef void (*command_parser_finished_callback_t)(const char*);

void command_parser_initialize(
    command_parser_error_callback_t error_callback,
    command_parser_finished_callback_t finish_callback
);

void command_parser_push(char c);

#endif // COMMAND_PARSER_H
