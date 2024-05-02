#ifndef APPLICATION_XMODEM_H
#define APPLICATION_XMODEM_H

// XMODEM uses the `timers.h` interface, so it _must_ be initialized before using this module.

#include "xmodem_parser.h"

// An XMODEM reception handler.
typedef struct xmodem {
    xmodem_parser_t parser;
} xmodem_t;

// Start an XMODEM reception by resetting its state and sending a NAK to the sender.
void xmodem_start(xmodem_t* xmodem);

// Inform XMODEM that a byte has been received.
void xmodem_push(xmodem_t* xmodem, uint8_t byte);

// Inform XMODEM that its timer timed out.
void xmodem_timeout(xmodem_t* xmodem);

#endif // APPLICATION_XMODEM_H
