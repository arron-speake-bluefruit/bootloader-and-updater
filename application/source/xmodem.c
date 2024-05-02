#include "xmodem.h"
#include "timers.h"
#include "event_queue.h"
#include <string.h>

enum {
    timeout_ms = 7000,
};

static void send_nak(void) {
    event_t event = {
        .type = event_type_usart_tx,
        .usart = 0x15,
    };
    event_queue_push(&event);
}

static void send_ack(void) {
    event_t event = {
        .type = event_type_usart_tx,
        .usart = 0x06,
    };
    event_queue_push(&event);
}

void xmodem_start(xmodem_t* xmodem) {
    event_t event = {
        .type = event_type_xmodem_timeout,
    };
    timers_start(timeout_ms, &event);

    xmodem_parser_reset(&xmodem->parser);
    send_nak();
}

void xmodem_push(xmodem_t* xmodem, uint8_t byte) {
    timers_restart();

    xmodem_status_t status = xmodem_parser_push(&xmodem->parser, byte);

    switch (status) {
        case xmodem_status_ok:
            // nothing to do
            break;

        case xmodem_status_packet_received: {
            // got a packet, send it onto the event queue
            event_t event = {
                .type = event_type_xmodem_packet,
            };
            memcpy(event.xmodem_packet, xmodem->parser.data, xmodem_packet_data_size);
            event_queue_push(&event);

            // and acknowledge the packet
            send_ack();

            break;
        }

        case xmodem_status_packet_invalid: {
            // invalid packet resets the parser, send a NAK
            send_nak();
            break;
        }

        case xmodem_status_complete: {
            // transmission over, inform the event queue and stop the timeout
            timers_cancel();
            send_ack();
            event_t event = {
                .type = event_type_xmodem_success,
            };
            event_queue_push(&event);
            break;
        }
    }
}

void xmodem_timeout(xmodem_t* xmodem) {
    xmodem_parser_timeout(&xmodem->parser);
    send_nak();

    event_t event = {
        .type = event_type_xmodem_timeout,
    };
    timers_start(timeout_ms, &event);
}
