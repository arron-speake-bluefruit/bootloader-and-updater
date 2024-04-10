#include "gpio.h"
#include "rcc.h"
#include "scb.h"
#include "syscfg.h"
#include "usart.h"
#include <string.h>

// Copy the application's vector table from the start of its image into SRAM so it is read afer
// a reset-to-SRAM.
static void copy_application_vectors_to_sram(void) {
    // Linker-defined symbols. These don't refer to anything coherent, and shouldn't be read, only
    // pointed to.
    extern const uint32_t _application_start;
    extern uint32_t sram_base; // TODO: put it in the linkerrrrrr

    const uint32_t* application_ptr = &application;
    uint32_t* sram_base = &sram_base;

    enum { vector_table_size = sizeof(uint32_t) * 1 }; // TODO: Use correct value.

    memcpy(sram_base, application_ptr, vector_table_size);
}

static void boot_to_application(void) {
    copy_application_vectors_to_sram();
    syscfg_set_mem_mode(syscfg_mem_mode_sram);
    scb_request_system_reset(SYSCFG_AIRCR_VECTKEY);
}

void main(void) {
    // Enable clocks for USART2 and the I/O port with the USART2 TX and RX pins.
    rcc_apb1_usart2_enable();
    rcc_ahb_iopa_en();

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

    boot_to_application();

    while (true) {}
}
