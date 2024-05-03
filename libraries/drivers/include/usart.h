#ifndef DRIVERS_USART_H
#define DRIVERS_USART_H

#include "driver_addresses.h"
#include <stdbool.h>

// Base address selection for interacting with USART peripherals.
typedef enum usart {
    usart1 = USART1_BASE_ADDRESS,
    usart2 = USART2_BASE_ADDRESS,
    usart3 = USART3_BASE_ADDRESS,
    usart4 = USART4_BASE_ADDRESS,
    usart5 = USART5_BASE_ADDRESS,
    usart6 = USART6_BASE_ADDRESS,
    usart7 = USART7_BASE_ADDRESS,
    usart8 = USART8_BASE_ADDRESS,
} usart_t;

// Set the clock divisor (`usartdiv`) for the given `usart`, for configuring its baud rate. This
// must only be called when the peripheral is disabled.
void usart_set_usartdiv(usart_t usart, uint16_t usartdiv);

// Enable `usart`, as well as its transmitter and receiver.
void usart_enable(usart_t usart);

// Disable `usart`, as well as its transmitter and receiver.
void usart_disable(usart_t usart);

// Returns true if the transmit register for `usart` is empty. False otherwise.
bool usart_transmit_register_is_empty(usart_t usart);

// Returns true if USART transmission is complete. False otherwise.
bool usart_tranmission_complete(usart_t usart);

// Perform an immediate write of `byte` onto `usart`. This must only be called when `usart`'s
// transmitter is enabled, and when the register is empty.
void usart_write(usart_t usart, uint8_t byte);

// Read a byte from the given usart peripheral. If there is no byte to be read, returns an
// undefined value.
uint8_t usart_read(usart_t usart);

// Return true if a byte can be read from the USART read register, false otherwise.
bool usart_can_read(usart_t usart);

// Enable the RXNE interrupt for the given USART. Triggers the USARTx global interrupt whenever its
// receive buffer is not empty.
void usart_enable_receive_interrupt(usart_t usart);

// Enable the IDLE interrupt for the given USART. Triggers the USARTx global interrupt whenever the
// RX line goes idle.
void usart_enable_idle_interrupt(usart_t usart);

// Clear the IDLE interrupt flag for the given USART.
void usart_clear_idle_line(usart_t usart);

// Enable the given USART's TXE interrupt.
void usart_enable_transmit_empty_interrupt(usart_t usart);

// Disable the given USART's TXE interrupt.
void usart_disable_transmit_empty_interrupt(usart_t usart);

#endif // DRIVERS_USART_H
