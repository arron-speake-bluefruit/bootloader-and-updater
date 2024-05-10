#ifndef EVENT_H
#define EVENT_H

#include "xmodem_parser.h"
#include "command_parser.h"

typedef enum event_type {
    // a byte has been received over the USART RX
    event_type_usart_rx,

    // a queued byte for USART transmission
    event_type_usart_tx,

    // a 128-byte XMODEM packets has been received
    event_type_xmodem_packet,

    // all XMODEM packets for the current file have been received
    event_type_xmodem_success,

    // the XMODEM timer timed out
    event_type_xmodem_timeout,

    // a command input has been entered
    event_type_command,

    // an invalid command was submitted
    event_type_invalid_command,

    // an unknown command was submitted
    event_type_unknown_command,
} event_type_t;

typedef enum event_command_t {
    event_command_help,
    event_command_boot_report,
    event_command_reset,
    event_command_xmodem,
    event_command_update_status,
} event_command_t;

typedef struct event {
    event_type_t type;

    union {
        uint8_t usart;
        // alignment hack, this is passes to flash_copy, which expects aligned halfwords
        uint8_t xmodem_packet[xmodem_packet_data_size] __attribute__(( aligned(2) ));
        event_command_t command;
        command_parser_error_t command_error;
    };
} event_t;

#endif // EVENT_H
