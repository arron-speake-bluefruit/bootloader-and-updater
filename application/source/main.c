#include "gpio.h"
#include "rcc.h"
#include "usart.h"
#include "nvic.h"
#include "flash.h"
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
        // usart_write(usart2, '@');
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

static void flash_page_erase(uint32_t address) {
    clear_flash_status();

    // Check that no flash memory operation is ongoing
    while (flash_is_busy()) {}

    // Set the PER bit in the FLASH_CR register.
    flash_set_operation(flash_operation_page_erase);

    // Program the FLASH_AR register to select a page to erase.
    flash_set_address(address);

    // Set the STRT bit in the FLASH_CR register
    flash_operation_start();

    // Note: The software should start checking if the BSY bit equals “0” at least one CPU cycle
    // after setting the STRT bit.

    // Wait for the flash operation to finish.
    bool hit = false;
    while (!hit) {
        uint32_t status = flash_status();
        if ((status & flash_status_bit_busy) == 0) {
            print("not busy\n");
            hit = true;
        } else {
            if ((status & flash_status_bit_programming_error) != 0) {
                print("programming error\n");
                hit = true;
            }

            if ((status & flash_status_bit_write_protect_error) != 0) {
                print("write protect error\n");
                hit = true;
            }

            if ((status & flash_status_bit_end_of_program) != 0) {
                print("end of program\n");
                hit = true;
            }
        }
    }
}

// Copy `count` halfwords from `source` to `destination`. Destination range MUST all be located inside flash.
// Before writing, performs an erase of all pages containing the destination range.
static void flash_overwrite(const uint16_t* source, uint16_t* destination, size_t count) {
    const uintptr_t page_size_bytes = 1024;

    uintptr_t destination_addr = (uintptr_t)destination;
    uintptr_t page_addr = (destination_addr / page_size_bytes) * page_size_bytes;
    uintptr_t page_count = count * 2 / page_size_bytes;
    uintptr_t end_page_addr = page_addr + (page_count * page_size_bytes);

    for (uintptr_t page = page_addr; page <= end_page_addr; page += page_size_bytes) {
        flash_page_erase(page);
        // TODO: Check for failure
    }

    for (uintptr_t i = 0; i < count; i++) {
        flash_set_operation(flash_operation_program);
        destination[i] = source[i];
        while (flash_is_busy()) {}

        // TODO: Check for failure
    }

    // TODO: Check for failure
}

// static void write_nibble(uint8_t n) {
//     while (!usart_transmit_register_is_empty(usart2)) {}

//     if (n < 10) {
//         usart_write(usart2, n + '0');
//     } else if (n < 16) {
//         usart_write(usart2, n - 10 + 'A');
//     } else {
//         usart_write(usart2, '?');
//     }
// }

void main(void) {
    switch_system_clock();

    // Configure flash.
    flash_enable_prefetch();
    flash_set_latency(flash_latency_one);

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
    print("I have written a new message here.\n");
    print("What's up?\n");

    while (true) {}
}
