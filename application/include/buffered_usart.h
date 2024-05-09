#ifndef BUFFERED_USART_H
#define BUFFERED_USART_H

#include <stdint.h>

// Initialize the USART TX buffer.
void buffered_usart_initialize(void);

// Push a byte to the USART TX buffer, to be transmitted in the near future. Blocks if the buffer
// is full.
void buffered_usart_push(uint8_t byte);

// Write the bytes of a string to the USART TX buffer. Blocks if the buffer becomes full
// during execution.
void buffered_usart_write(const char* string);

// Block until the USART peripheral has finished transmitting and the ringbuffer is empty.
void buffered_usart_flush(void);

#endif // BUFFERED_USART_H
