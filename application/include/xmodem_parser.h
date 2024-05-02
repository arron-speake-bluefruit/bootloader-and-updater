#ifndef APPLICATION_XMODEM_PARSER_H
#define APPLICATION_XMODEM_PARSER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

enum {
    xmodem_packet_data_size = 128,
};

typedef enum xmodem_status {
    // A byte has been pushed into the parser successfully.
    xmodem_status_ok,

    // The final byte of a packet has been received, and the `data` field contains valid packet
    // contents until the next call onto the structure.
    // The callback handler should transmit an ACK.
    xmodem_status_packet_received,

    // A packet was invalid and must be re-transmitted.
    // The callback handler should transmit a NAK.
    xmodem_status_packet_invalid,

    // The transfer failed because of a timeout / TODO
    xmodem_status_failed,

    // The transfer has finished. The previous packet was the final one.
    xmodem_status_complete,
} xmodem_status_t;

// A parser for incoming XMODEM packets.
typedef struct xmodem_parser {
    // The packet number expected to be received.
    uint8_t expected_packet;

    // The byte offset into the packet expected to be received.
    uint8_t byte_index;

    // The checksum of the received data so far.
    uint8_t current_checksum;

    // If set, the XMODEM parser won't trigger mode callbacks.
    bool done;

    // Buffer for storing partially received packet data.
    uint8_t data[xmodem_packet_data_size];
} xmodem_parser_t;

// Reset an XMODEM struct to expect the
void xmodem_parser_reset(xmodem_parser_t* xmodem);

// Push a newly received by from the USART onto the XMODEM parser.
xmodem_status_t xmodem_parser_push(xmodem_parser_t* xmodem, uint8_t byte);

// Inform the parser that a previously configured timeout has elapsed.
void xmodem_parser_timeout(xmodem_parser_t* xmodem);

#endif // APPLICATION_XMODEM_PARSER_H
