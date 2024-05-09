#include "boot_report.h"
#include "buffered_usart.h"
#include "command_parser.h"
#include "event_queue.h"
#include "flash.h"
#include "git_version.h"
#include "rcc.h"
#include "timers.h"
#include "xmodem.h"
#include "scb.h"
#include <string.h>

static boot_report_t boot_report;
static bool is_xmodem_mode;
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
    if (is_xmodem_mode) {
        xmodem_push(&xmodem, byte);
    } else {
        command_parser_push((char)byte);
    }
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
    is_xmodem_mode = false;
}

static void handle_xmodem_timeout_event(void) {
    xmodem_timeout(&xmodem);
}

static void command_help(void) {
    buffered_usart_write("COMMANDS:\n");
    buffered_usart_write("  boot-report - display the reported behaviour from the bootloader\n");
    buffered_usart_write("  help - display this help string\n");
    buffered_usart_write("  reset - perform a soft reset on the device\n");
    buffered_usart_write("  xmodem - switch to XMODEM mode\n");
}

static const char* get_boot_report_string(boot_report_t boot_report) {
    switch (boot_report) {
        case boot_report_typical:
            return "typical boot";
        case boot_report_updated:
            return "an update was successful";
        case boot_report_failed:
            return "an update was attempted, but failed";
    }
    return "?";
}

static void command_boot_report(void) {
    const char* string = get_boot_report_string(boot_report);

    buffered_usart_write("boot report: ");
    buffered_usart_write(string);
    buffered_usart_write("\n");
}

static void command_reset(void) {
    buffered_usart_write("performing system reset request...\n");
    buffered_usart_flush();

    scb_request_system_reset(SYSCFG_AIRCR_VECTKEY);
}

static void command_xmodem(void) {
    buffered_usart_write("switching to XMODEM mode\n");
    is_xmodem_mode = true;
    xmodem_start(&xmodem);
}

static void handle_command_event(event_command_t command) {
    switch (command) {
        case event_command_help:
            command_help();
            break;
        case event_command_boot_report:
            command_boot_report();
            break;
        case event_command_reset:
            command_reset();
            break;
        case event_command_xmodem:
            command_xmodem();
            break;
    }
}

static const char* get_command_parser_error_string(command_parser_error_t error) {
    switch (error) {
        case command_parser_error_too_long:
            return "command was too long";
    }
    return "?";
}

static void handle_invalid_command_event(command_parser_error_t error) {
    const char* string = get_command_parser_error_string(error);

    buffered_usart_write("error: failed to parse command: ");
    buffered_usart_write(string);
    buffered_usart_write("\n");
}

static void handle_unknown_command_event(void) {
    buffered_usart_write("error: given command was unknown\n");
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
        case event_type_command:
            handle_command_event(event->command);
            break;
        case event_type_invalid_command:
            handle_invalid_command_event(event->command_error);
            break;
        case event_type_unknown_command:
            handle_unknown_command_event();
            break;
    }
}

static void on_command_error(command_parser_error_t error) {
    event_t event = {
        .type = event_type_invalid_command,
        .command_error = error,
    };
    event_queue_push(&event);
}

bool get_command_by_name(const char* name, event_command_t* command) {
    if (strcmp(name, "help") == 0) {
        *command = event_command_help;
        return true;
    } else if (strcmp(name, "boot-report") == 0) {
        *command = event_command_boot_report;
        return true;
    } else if (strcmp(name, "reset") == 0) {
        *command = event_command_reset;
        return true;
    } else if (strcmp(name, "xmodem") == 0) {
        *command = event_command_xmodem;
        return true;
    } else {
        return false;
    }
}

static void on_command_finished(const char* command_name) {
    event_command_t command;
    if (get_command_by_name(command_name, &command)) {
        event_t event = {
            .type = event_type_command,
            .command = command,
        };
        event_queue_push(&event);
    } else {
        event_t event = { .type = event_type_unknown_command };
        event_queue_push(&event);
    }
}

void main(boot_report_t new_boot_report) {
    boot_report = new_boot_report;
    is_xmodem_mode = false;

    // Enable flash read wait state BEFORE increasing system clock speed.
    flash_set_latency(flash_latency_one);
    flash_enable_prefetch();

    switch_system_clock();

    event_queue_initialize();

    buffered_usart_initialize();

    command_parser_initialize(on_command_error, on_command_finished);

    timers_initialize();

    // Write a welcome message.
    buffered_usart_write("application started\ncommit: ");
    buffered_usart_write(git_version);
    buffered_usart_write("\n");

    while (true) {
        event_t event;
        if (event_queue_pop(&event)) {
            handle_event(&event);
        }
    }
}
