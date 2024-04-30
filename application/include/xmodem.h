#ifndef APPLICATION_XMODEM_H
#define APPLICATION_XMODEM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Parser potion of an XMODEM implementation. Integration with a USART or TIM peripheral is not
// handled here. Retry-counting is not implemented (yet).

typedef enum xmodem_status {
    // A byte has been received, regardless of validity.
    xmodem_status_byte_received,

    // A packet has been received, associated data points to 128 bytes of received data.
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

// Callback type for when an XMODEM event occurs.
typedef void (xmodem_callback_t)(xmodem_status_t status, const void* data);

// A parser for incoming XMODEM packets.
typedef struct xmodem {
    // The packet number expected to be received.
    uint8_t expected_packet;

    // The byte offset into the packet expected to be received.
    uint8_t packet_index;

    // The checksum of the received data so far.
    uint8_t current_checksum;

    // If set, the XMODEM parser won't trigger mode callbacks.
    bool done;

    // Buffer for storing partially received packet data.
    uint8_t data[128];

    // Called when an event occurs (see `xmodem_status_t`).
    xmodem_callback_t callback;
} xmodem_t;

// Reset an XMODEM struct to expect the
void xmodem_reset(xmodem_t* xmodem, xmodem_callback_t callback);

// Push a newly received by from the USART onto the XMODEM parser.
void xmodem_push(xmodem_t* xmodem, uint8_t byte);

// Inform the parser that a previously configured timeout has elapsed.
void xmodem_timeout(xmodem_t* xmodem, uint8_t byte);

#endif // APPLICATION_XMODEM_H
