#include "gpio.h"
#include "rcc.h"
#include "usart.h"
#include "nvic.h"
#include "vector_table.h"
#include "command_parser.h"
#include "ringbuffer.h"

static void on_command_error(command_parser_error_t error) {
    (void)error;
}

static void on_command_finish(const char* command) {
    (void)command;
}

// Change the system clock to the PLL, from 8Mhz to 48MHz.
static void switch_system_clock(void) {
    // Multiplication factors is off by 2 (MF=10 multiplies frequency by 12, eg)
    // PLL has builtin /2 prescaler from the HSI.
    rcc_set_pll_multiplication_factor(10); // 8MHz / 2 * 12 = 48MHz PLL frequency.
    rcc_enable_pll();

    // Busy-wait until PLL clock is locked.
    // TODO: Use PLL ready interrupt (see RCC_CIR).
    while (!rcc_pll_clock_ready()) {}

    rcc_switch_system_clock(rcc_system_clock_pll);
}

static void usart2_global_interrupt(void) {
    if (usart_can_read(usart2)) {
        // Read interrupt.
        uint8_t byte = usart_read(usart2);
        usart_write(usart2, byte);
    } else {
        // Only other enable interrupt is IDLE.
        usart_clear_idle_line(usart2);
        usart_write(usart2, '@');
    }
}

void main(void) {
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

    command_parser_initialize(on_command_error, on_command_finish);

    // Enable USART2.
    usart_enable(usart2);
    usart_enable_receive_interrupt(usart2);
    usart_enable_idle_interrupt(usart2);

    // Write a welcome message.
    const char* const message = "Hello, world.\n";
    for (const char* c = message; *c != '\0'; c++) {
        // Busy-wait until the next byte can be transmitted.
        while (!usart_transmit_register_is_empty(usart2)) {}

        // Transmit the next byte.
        usart_write(usart2, (uint8_t)*c);
    }

    while (true) {}
}
