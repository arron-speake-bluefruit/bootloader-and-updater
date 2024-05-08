#include "buffered_usart.h"
#include "event_queue.h"
#include "flash.h"
#include "git_version.h"
#include "rcc.h"
#include "timers.h"
#include "xmodem.h"
#include <string.h>

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

static void handle_usart_rx_event(uint8_t byte) {
    xmodem_push(&xmodem, byte);
}

static void handle_usart_tx_event(uint8_t byte) {
    buffered_usart_push(byte);
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

    event_queue_initialize();

    buffered_usart_initialize();

    // command_parser_initialize(on_command_error, on_command_finish);

    timers_initialize();

    // Write a welcome message.
    buffered_usart_write("application started\ncommit: ");
    buffered_usart_write(git_version);
    buffered_usart_write("\n");

    buffered_usart_write("switching to XMODEM mode\n");
    xmodem_start(&xmodem);

    while (true) {
        event_t event;
        if (event_queue_pop(&event)) {
            handle_event(&event);
        }
    }
}
