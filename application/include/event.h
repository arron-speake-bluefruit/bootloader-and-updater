#ifndef EVENT_H
#define EVENT_H

#include "xmodem_parser.h"

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
} event_type_t;

typedef struct event {
    event_type_t type;

    union {
        uint8_t usart;
        uint8_t xmodem_packet[xmodem_packet_data_size];
    };
} event_t;

/*
typedef struct usart_event {
    bool is_idle;
    uint8_t data;
} usart_event_t;
*/

#endif // EVENT_H
