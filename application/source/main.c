#include "gpio.h"
#include "rcc.h"
#include "usart.h"
#include "nvic.h"
#include "flash.h"
#include "vector_table.h"
#include "command_parser.h"
#include "ringbuffer.h"
#include "git_version.h"
#include "timers.h"
#include "event_queue.h"
#include "xmodem.h"
#include <string.h>

// static void on_command_error(command_parser_error_t error) {
//     event.is_error = true;
//     event.error = error;
//     has_event = true;
// }

// static void on_command_finish(const char* command) {
//     size_t length = strlen(command);
//     event.is_error = false;
//     // TODO: There's no bounds-check here.
//     memcpy((void*)event.command, command, length + 1);
//     has_event = true;
// }

static xmodem_t xmodem;

static uint8_t xmodem_rx_buffer[1024];
static size_t xmodem_rx_buffer_size = 0;

// Change the system clock to the PLL, from 8Mhz to 48MHz.
static void switch_system_clock(void) {
    // Multiplication factors is off by 2 (MF=10 multiplies frequency by 12, eg)
    // PLL has builtin /2 prescaler from the HSI.
    rcc_set_pll_multiplication_factor(10); // 8MHz / 2 * 12 = 48MHz PLL frequency.
    rcc_enable_pll();

    // Busy-wait until PLL clock is locked.
    while (!rcc_pll_clock_ready()) {}

    rcc_switch_system_clock(rcc_system_clock_pll);
}

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

void print(const char* message) {
    for (const char* c = message; *c != '\0'; c++) {
        // Busy-wait until the next byte can be transmitted.
        while (!usart_transmit_register_is_empty(usart2)) {}

        // Transmit the next byte.
        usart_write(usart2, (uint8_t)*c);
    }
}

static void handle_usart_rx_event(uint8_t byte) {
    xmodem_push(&xmodem, byte);
}

static void handle_usart_tx_event(uint8_t byte) {
    while (!usart_transmit_register_is_empty(usart2)) {}
    usart_write(usart2, byte);
}

static void handle_xmodem_packet_event(const void* data) {
    // TODO: bounds check!
    if (xmodem_rx_buffer_size < 1024) {
        memcpy(&xmodem_rx_buffer[xmodem_rx_buffer_size], data, xmodem_packet_data_size);
        xmodem_rx_buffer_size += xmodem_packet_data_size;
    }
}

static void handle_xmodem_success_event(void) {
    // nothing
}

void handle_xmodem_timeout_event(void) {
    xmodem_timeout(&xmodem);
}

static void handle_event(const event_t* event) {
    switch (event->type) {
        case event_type_usart_rx:
            handle_usart_rx_event(event->usart);
            break;
        case event_type_usart_tx:
            handle_usart_tx_event(event->usart);
            break;
        case event_type_xmodem_packet:
            handle_xmodem_packet_event(event->xmodem_packet);
            break;
        case event_type_xmodem_success:
            handle_xmodem_success_event();
            break;
        case event_type_xmodem_timeout:
            handle_xmodem_timeout_event();
            break;
    }
}

void main(void) {
    // Enable flash read wait state BEFORE increasing system clock speed.
    flash_set_latency(flash_latency_one);
    flash_enable_prefetch();

    switch_system_clock();

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

    vector_table_set(vector_table_index_usart2, usart2_global_interrupt);
    nvic_enable_usart2_global_interrupt();

    event_queue_initialize();
    // command_parser_initialize(on_command_error, on_command_finish);

    // Enable USART2.
    usart_enable(usart2);
    usart_enable_receive_interrupt(usart2);
    usart_enable_idle_interrupt(usart2);

    timers_initialize();

    // Write a welcome message.
    print("application started\ncommit: ");
    print(git_version);
    print("\n");

    print("switching to XMODEM mode\n");
    xmodem_start(&xmodem);

    while (true) {
        event_t event;
        if (event_queue_pop(&event)) {
            handle_event(&event);
        }
    }
}
