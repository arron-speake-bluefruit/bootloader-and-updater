#include "xmodem_parser.h"

enum {
    start_of_heading = (uint8_t)0x01,
    end_of_text = (uint8_t)0x04,
};

void xmodem_parser_reset(xmodem_parser_t* xmodem) {
    xmodem->expected_packet = 1;
    xmodem->byte_index = 0;
}

xmodem_status_t xmodem_parser_push(xmodem_parser_t* xmodem, uint8_t byte) {
    uint8_t index = xmodem->byte_index;

    if (index == 0) {
        // Expecting an SOH or EOT byte.
        if (byte == start_of_heading) {
            xmodem->byte_index += 1;
            return xmodem_status_ok;
        } else if (byte == end_of_text) {
            return xmodem_status_complete;
        } else {
            return xmodem_status_packet_invalid;
        }
    } else if (index == 1) {
        // Expecting the block number of the current packet.
        if (byte == xmodem->expected_packet) {
            xmodem->byte_index += 1;
            return xmodem_status_ok;
        } else {
            return xmodem_status_packet_invalid;
            xmodem->byte_index = 0;
        }
    } else if (index == 2) {
        // Expecting the block number's complement.
        uint8_t expected_complement = (uint8_t)255 - xmodem->expected_packet;
        if (byte == expected_complement) {
            xmodem->byte_index += 1;
            xmodem->current_checksum = 0;
            return xmodem_status_ok;
        } else {
            return xmodem_status_packet_invalid;
            xmodem->byte_index = 0;
        }
    } else if (index < (xmodem_packet_data_size + 3)) {
        // Data byte, read it directly.
        size_t buffer_index = (size_t)index - 3; // subtract heading size
        xmodem->data[buffer_index] = byte;
        xmodem->current_checksum += byte; // allow overflow
        xmodem->byte_index += 1;
        return xmodem_status_ok;
    } else {
        // Checksum
        if (byte == xmodem->current_checksum) {
            xmodem->byte_index = 0;
            xmodem->expected_packet += 1;
            return xmodem_status_packet_received;
        } else {
            xmodem->byte_index = 0;
            return xmodem_status_packet_invalid;
        }
    }
}

void xmodem_parser_timeout(xmodem_parser_t* xmodem) {
    xmodem->byte_index = 0;
}
