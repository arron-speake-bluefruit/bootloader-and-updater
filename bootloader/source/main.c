#include "gpio.h"
#include "rcc.h"
#include "scb.h"
#include "syscfg.h"
#include "usart.h"
#include <string.h>

// Copy the application's vector table from the start of its image into SRAM so it is read afer
// a reset-to-SRAM.
static void copy_application_vectors_to_sram(void) {
    extern const uint32_t APPLICATION_ORIGIN;
    extern uint32_t _app_vectors[];

    const size_t vector_table_length = 48;
    const size_t vector_table_size = sizeof(uint32_t) * vector_table_length;

    memcpy(_app_vectors, &APPLICATION_ORIGIN, vector_table_size);
}

static void jump_to_application_reset_handler(void) {
    extern uint32_t _app_vectors[];

    uint32_t stack_pointer = _app_vectors[0];
    uint32_t reset_handler_vector = _app_vectors[1];
    uint32_t link_register = 0xFFFFFFFF;

    // Unfortunately, a little assembly is required for this.
    // This places `stack_pointer` into the SP register, resets the link register, and then jumps
    // to `reset_handler_vector`.
    __asm__ (
        "mov sp, %0\n"
        "mov lr, %1\n"
        "bx %2\n"
        :
        : "r" (stack_pointer), "r" (link_register), "r" (reset_handler_vector)
    );
}

static void boot_to_application(void) {
    copy_application_vectors_to_sram();
    syscfg_set_mem_mode(syscfg_mem_mode_sram);
    jump_to_application_reset_handler();
}

void main(void) {
    // Enable clocks for USART2 and the I/O port with the USART2 TX and RX pins.
    rcc_apb1_usart2_enable();
    rcc_ahb_iopa_enable();

    // Configure the USART RX/TX pins to the correct alternative function.
    gpio_set_mode(gpio_a, 2, gpio_mode_alternative);
    gpio_set_mode(gpio_a, 3, gpio_mode_alternative);
    gpio_set_alternate_function(gpio_a, 2, 1); // USART2_TX
    gpio_set_alternate_function(gpio_a, 3, 1); // USART2_RX

    // Configure USART2's baud rate.
    const uint32_t clock_frequency = 8 * 1000 * 1000;
    const uint32_t target_baud_rate = 9600;
    const uint16_t usart_clock_div = (uint16_t)(clock_frequency / target_baud_rate);
    usart_set_usartdiv(usart2, usart_clock_div);

    // Enable USART2.
    usart_enable(usart2);

    // Write a welcome message.
    const char* const message = "[bootloader] ready, jumping to application\n";
    for (const char* c = message; *c != '\0'; c++) {
        // Busy-wait until the next byte can be transmitted.
        while (!usart_transmit_register_is_empty(usart2)) {}

        // Transmit the next byte.
        usart_write(usart2, (uint8_t)*c);
    }

    // Wait until USART2 has finished transmitting.
    while (!usart_tranmission_complete(usart2)) {}

    // Turn the peripherals off before booting to the application.
    usart_disable(usart2);
    rcc_ahb_iopa_disable();
    rcc_apb1_usart2_disable();

    boot_to_application();

    while (true) {}
}
