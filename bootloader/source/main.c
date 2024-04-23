#include "gpio.h"
#include "rcc.h"
#include "scb.h"
#include "syscfg.h"
#include "usart.h"
#include "flash.h"
#include <string.h>

// Copy the application's vector table from the start of its image into SRAM so it is read afer
// a reset-to-SRAM.
static void copy_application_vectors_to_sram(void) {
    extern uint32_t APPLICATION_ORIGIN;
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

static void print(const char* message) {
    for (const char* c = message; *c != '\0'; c++) {
        // Busy-wait until the next byte can be transmitted.
        while (!usart_transmit_register_is_empty(usart2)) {}

        // Transmit the next byte.
        usart_write(usart2, (uint8_t)*c);
    }
}

static void flash_overwrite(const uint16_t* source, uint16_t* destination, size_t count) {
    const uintptr_t page_size_bytes = 1024;

    // TODO: Make sure this page range calculation is correct.
    uintptr_t destination_addr = (uintptr_t)destination;
    uintptr_t page_addr = (destination_addr / page_size_bytes) * page_size_bytes;
    uintptr_t page_count = count * 2 / page_size_bytes;
    uintptr_t end_page_addr = page_addr + (page_count * page_size_bytes);

    for (uintptr_t page = page_addr; page < end_page_addr; page += page_size_bytes) {
        // Check that no flash memory operation is ongoing
        while (flash_is_busy()) {}

        // Set the PER bit in the FLASH_CR register.
        flash_set_operation(flash_operation_page_erase);

        // Program the FLASH_AR register to select a page to erase.
        flash_set_address(page);

        // Set the STRT bit in the FLASH_CR register
        flash_operation_start();

        // Note: The software should start checking if the BSY bit equals “0” at least one CPU cycle
        // after setting the STRT bit.

        // Wait for the flash operation to finish.
        bool hit = false;
        while (!hit) {
            uint32_t status = flash_status();
            if ((status & flash_status_bit_busy) == 0) {
                print("[bootloader] erased block\n");
                hit = true;
            } else {
                if ((status & flash_status_bit_programming_error) != 0) {
                    print("[bootloader] programming error during erase\n");
                    hit = true;
                }

                if ((status & flash_status_bit_write_protect_error) != 0) {
                    print("[bootloader] write protect error during erase\n");
                    hit = true;
                }

                if ((status & flash_status_bit_end_of_program) != 0) {
                    print("[bootloader] end of program during erase\n");
                    hit = true;
                }
            }
        }
        // TODO: Proper error handling.
    }

    print("[bootloader] writing...\n");
    for (uintptr_t i = 0; i < count; i++) {
        flash_set_operation(flash_operation_program);
        destination[i] = source[i];
        while (flash_is_busy()) {}

        // TODO: Check for failure
    }

    // TODO: Check for failure
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
    const uint32_t target_baud_rate = 38400;
    const uint16_t usart_clock_div = (uint16_t)(clock_frequency / target_baud_rate);
    usart_set_usartdiv(usart2, usart_clock_div);

    // Enable USART2.
    usart_enable(usart2);

    // Perform bootloader logic.

    bool should_update = true; // TODO

    if (should_update) {
        extern uint32_t APPLICATION_SIZE;
        extern uint32_t APPLICATION_ORIGIN;
        extern uint32_t UPDATE_REGION_ORIGIN;

        flash_unlock(FLASH_KEY1, FLASH_KEY2);
        print("[bootloader] unlocked flash\n");

        print("[bootloader] starting update...\n");
        const uint16_t* source = (const uint16_t*)&UPDATE_REGION_ORIGIN;
        uint16_t* destination = (uint16_t*)&APPLICATION_ORIGIN;
        size_t count = (size_t)&APPLICATION_SIZE / 2;
        flash_overwrite(source, destination, count);
        print("[bootloader] update finished\n");

        flash_lock();
        print("[bootloader] locked flash\n");
    }

    print("[bootloader] ready, jumping to application...\n");

    // Wait until USART2 has finished transmitting.
    while (!usart_tranmission_complete(usart2)) {}

    // Turn the peripherals off before booting to the application.

    usart_disable(usart2);
    rcc_ahb_iopa_disable();
    rcc_apb1_usart2_disable();

    boot_to_application();

    while (true) {}
}
