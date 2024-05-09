#include "gpio.h"
#include "rcc.h"
#include "usart.h"
#include "flash_copy.h"
#include "boot.h"
#include "git_version.h"

// Print all bytes of `message` into USART2 TX.
static void print(const char* message) {
    for (const char* c = message; *c != '\0'; c++) {
        // Busy-wait until the next byte can be transmitted.
        while (!usart_transmit_register_is_empty(usart2)) {}

        // Transmit the next byte.
        usart_write(usart2, (uint8_t)*c);
    }
}

// Setup USART2 for I/O.
static void configure_usart2(void) {
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
}

static void wait_until_usart2_transmission_complete(void) {
    while (!usart_tranmission_complete(usart2)) {}
}

// Shutdown USART2 before booting.
static void reset_usart2(void) {
    usart_disable(usart2);
    rcc_ahb_iopa_disable();
    rcc_apb1_usart2_disable();
}

// Copy data from the update region into the application region.
static void perform_firmware_update(void) {
    // Refer to symbols defined in the linker script for region size & origin information.
    extern uint32_t APPLICATION_SIZE;
    extern uint32_t APPLICATION_ORIGIN;
    extern uint32_t UPDATE_REGION_ORIGIN;

    uint32_t source_page = (uint32_t)&UPDATE_REGION_ORIGIN;
    uint32_t destination_page = (uint32_t)&APPLICATION_ORIGIN;
    uint32_t byte_count = (size_t)&APPLICATION_SIZE;
    uint32_t page_count = byte_count / flash_page_size; // round up the number of pages
    uint32_t halfword_count = byte_count / 2;

    print("[bootloader] erasing application\n");
    bool did_erase = flash_erase_pages(destination_page, page_count);
    if (!did_erase) {
        print("[bootloader] ERROR: failed to erase pages in application region\n");
    }

    print("[bootloader] copying new application\n");
    flash_copy((const uint16_t*)source_page, (uint16_t*)destination_page, halfword_count);

    print("[bootloader] finished\n");
}

void main(void) {
    configure_usart2();

    print("[bootloader] started, commit: ");
    print(git_version);
    print("\n");

    boot_report_t boot_report = boot_report_typical;

    bool should_update = true; // TODO

    if (should_update) {
        // TODO: Check application image CRC-32 before update.
        perform_firmware_update();
        boot_report = boot_report_updated;
    }

    print("[bootloader] about to boot\n");
    wait_until_usart2_transmission_complete();
    reset_usart2();
    boot_to_application(boot_report);
}
