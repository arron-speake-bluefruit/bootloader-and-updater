#include "buffered_usart.h"
#include "event_queue.h"
#include "gpio.h"
#include "nvic.h"
#include "rcc.h"
#include "usart.h"
#include "vector_table.h"

static void usart2_global_interrupt(void) {
    if (usart_can_read(usart2)) {
        // Read interrupt.
        uint8_t byte = usart_read(usart2);
        event_t event = {
            .type = event_type_usart_rx,
            .usart = byte,
        };
        event_queue_push(&event);
    } else {
        // Only other enable interrupt is IDLE.
        usart_clear_idle_line(usart2);
    }
}

void buffered_usart_initialize(void) {
    // Enable clocks for USART2 and the I/O port with the USART2 TX and RX pins.
    rcc_apb1_usart2_enable();
    rcc_ahb_iopa_enable();

    // Configure the USART RX/TX pins to the correct alternative function.
    gpio_set_mode(gpio_a, 2, gpio_mode_alternative);
    gpio_set_mode(gpio_a, 3, gpio_mode_alternative);
    gpio_set_alternate_function(gpio_a, 2, 1); // USART2_TX
    gpio_set_alternate_function(gpio_a, 3, 1); // USART2_RX

    // Configure USART2's baud rate.
    const uint32_t clock_frequency = 48 * 1000 * 1000;
    const uint32_t target_baud_rate = 38400;
    const uint16_t usart_clock_div = (uint16_t)(clock_frequency / target_baud_rate);
    usart_set_usartdiv(usart2, usart_clock_div);

    // Set `usart2_global_interrupt` as the USART2 interrupt handler.
    vector_table_set(vector_table_index_usart2, usart2_global_interrupt);
    nvic_enable_usart2_global_interrupt();

    // Enable USART2.
    usart_enable(usart2);
    usart_enable_receive_interrupt(usart2);
    usart_enable_idle_interrupt(usart2);
}

void buffered_usart_push(uint8_t byte) {
    // TODO: Ringbuffer implementation

    // Busy-wait until the next byte can be transmitted.
    while (!usart_transmit_register_is_empty(usart2)) {}

    // Transmit the next byte.
    usart_write(usart2, byte);
}

void buffered_usart_write(const char* string) {
    for (const char* c = string; *c != '\0'; c++) {
        buffered_usart_push(*c);
    }
}

void buffered_usart_flush(void) {
    // TODO: When a ringbuffer is added, this needs to wait for the ringbuffer to empty.

    // Busy-wait until there are no more bytes to send.
    while (!usart_transmit_register_is_empty(usart2)) {}

    // Busy wait until transmission of the current byte is complete.
    while (!usart_tranmission_complete(usart2)) {}
}
