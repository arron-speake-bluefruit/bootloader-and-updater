#ifndef DRIVERS_USART_H
#define DRIVERS_USART_H

#include <stdbool.h>
#include <stdint.h>

// Base address selection for interacting with USART peripherals.
typedef enum usart {
    usart1 = ((uintptr_t)0x40013800),
    usart2 = ((uintptr_t)0x40004400),
    usart3 = ((uintptr_t)0x40004800),
    usart4 = ((uintptr_t)0x40004C00),
    usart5 = ((uintptr_t)0x40005000),
    usart6 = ((uintptr_t)0x40011400),
    usart7 = ((uintptr_t)0x40011800),
    usart8 = ((uintptr_t)0x40011C00),
} usart_t;

// Set the clock divisor (`usartdiv`) for the given `usart`, for configuring its baud rate. This
// must only be called when the peripheral is disabled.
void usart_set_usartdiv(usart_t usart, uint16_t usartdiv);

// Enable `usart`, as well as its transmitter and receiver.
void usart_enable(usart_t usart);

// Returns true if the transmit register for `usart` is empty. False otherwise.
bool usart_transmit_register_is_empty(usart_t usart);

// Returns true if USART transmission is complete. False otherwise.
bool usart_tranmission_complete(usart_t usart);

// Perform an immediate write of `byte` onto `usart`. This must only be called when `usart`'s
// transmitter is enabled, and when the register is empty.
void usart_write(usart_t usart, uint8_t byte);

#endif // DRIVERS_USART_H
