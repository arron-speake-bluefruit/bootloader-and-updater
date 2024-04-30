#include "gpio.h"
#include "rcc.h"
#include "usart.h"
#include "nvic.h"
#include "flash.h"
#include "vector_table.h"
#include "command_parser.h"
#include "ringbuffer.h"
#include "git_version.h"
#include "tim.h"
#include <string.h>

typedef struct event {
    bool is_error;

    union {
        command_parser_error_t error;
        char command[32];
    };
} event_t;

static volatile event_t event;
static volatile bool has_event;

static void on_command_error(command_parser_error_t error) {
    event.is_error = true;
    event.error = error;
    has_event = true;
}

static void on_command_finish(const char* command) {
    size_t length = strlen(command);
    event.is_error = false;
    // TODO: There's no bounds-check here.
    memcpy((void*)event.command, command, length + 1);
    has_event = true;
}

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
        command_parser_push(byte);
    } else {
        // Only other enable interrupt is IDLE.
        usart_clear_idle_line(usart2);
    }
}

static void print(const char* message) {
    for (const char* c = message; *c != '\0'; c++) {
        // Busy-wait until the next byte can be transmitted.
        while (!usart_transmit_register_is_empty(usart2)) {}

        // Transmit the next byte.
        usart_write(usart2, (uint8_t)*c);
    }
}

static void tim6_global_interrupt(void) {
    tim_clear_interrupt_flag(tim6);
    print("tick\n");
}

void main(void) {
    switch_system_clock();

    // Configure flash.
    flash_enable_prefetch();
    flash_set_latency(flash_latency_one);

    // Enable clocks for USART2 and the I/O port with the USART2 TX and RX pins.
    rcc_apb1_usart2_enable();
    rcc_ahb_iopa_enable();
    rcc_apb1_tim7_enable();

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
    vector_table_set(vector_table_index_tim6, tim6_global_interrupt);
    nvic_enable_usart2_global_interrupt();
    nvic_enable_tim6_global_interrupt();

    command_parser_initialize(on_command_error, on_command_finish);

    // Enable USART2.
    usart_enable(usart2);
    usart_enable_receive_interrupt(usart2);
    usart_enable_idle_interrupt(usart2);

    // Configure TIM6
    tim_enable_interrupt(tim6);
    tim_set_autoreload_value(tim6, 1000);
    tim_set_prescaler(tim6, 48000);

    // Write a welcome message.
    print("application started\ncommit: ");
    print(git_version);
    print("\n");

    tim_enable(tim6);

    while (true) {
        if (has_event) {
            has_event = false;
            if (event.is_error) {
                print("error: ");
                print(command_parser_error_as_string(event.error));
                print("\n");
            } else {
                print("command: ");
                print((const char*)event.command);
                print("\n");
            }
        }
    }
}
