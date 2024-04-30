#include "xmodem.h"

enum {
    start_of_heading = (uint8_t)0x01,
    end_of_text = (uint8_t)0x03,
}

void xmodem_reset(xmodem_t* xmodem, xmodem_callback_t callback) {
    xmodem->expected_packet = 1;
    xmodem->packet_index = 0;
    xmodem->callback = callback;
    xmodem->done = false;
}

void xmodem_push(xmodem_t* xmodem, uint8_t byte) {
    uint8_t index = xmodem->packet_index;

    if (xmodem->done) {
        return;
    }

    if (index == 0) {
        // Expecting an SOH or EOT byte.
        if (byte == start_of_heading) {
            xmodem->packet_index += 1;
        } else if (byte == end_of_text) {
            xmodem->callback(xmodem_status_complete, NULL);
            xmodem->done = true;
        } else {
            xmodem->callback(xmodem_status_packet_invalid, NULL);
        }
    } else if (index == 1) {
        // Expecting the block number of the current packet.
        if (byte == xmodem->expected_packet) {
            xmodem->packet_index += 1;
        } else {
            xmodem->callback(xmodem_status_packet_invalid, NULL);
            xmodem->packet_index = 0;
        }
    } else if (index == 2) {
        // Expecting the block number's complement.
        uint8_t expected_complement = (uint8_t)255 - xmodem->expected_packet;
        if (byte == expected_complement) {
            xmodem->packet_index += 1;
            xmodem->current_checksum = 0;
        } else {
            xmodem->callback(xmodem_status_packet_invalid, NULL);
            xmodem->packet_index = 0;
        }
    } else if (index < 131) {
        // Data byte, read it directly.
        size_t buffer_index = (size_t)byte - 3; // subtract heading size
        xmodem->data[buffer_index] = byte;
        xmodem->current_checksum += byte; // allow overflow
        xmodem->packet_index += 1;
    } else {
        // Checksum
        if (byte == xmodem->current_checksum) {
            xmodem->callback(xmodem_status_received, xmodem->data);
            xmodem->expected_packet += 1; // TODO: Check for overflow?
            xmodem->packet_index = 0;
        } else {
            xmodem->callback(xmodem_status_packet_invalid, NULL);
            xmodem->packet_index = 0;
        }
    }
}

void xmodem_timeout(xmodem_t* xmodem, uint8_t byte) {
    // TODO: Se
}
