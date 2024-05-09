#include <stdbool.h>
#include <stdint.h>

extern uint32_t _estack;

// The bootloader and application's reset handler are different, so these are provided in the
// executable projects directly.
extern void reset_handler(void);

void nmi_handler(void) {
    while (true) { }
}

void hardfault_handler(void) {
    while (true) { }
}

void menmanage_handler(void) {
    while (true) { }
}

void busfault_handler(void) {
    while (true) { }
}

void usagefault_handler(void) {
    while (true) { }
}

__attribute__ ((section(".vectors")))
const uintptr_t vectors[] = {
    (uintptr_t)&_estack, // 0x0000'0000 Reset value of main stack pointer
    (uintptr_t)reset_handler, // 0x0000'0004 - Reset
    (uintptr_t)nmi_handler, // 0x0000'0008 - Non maskable interrupt
    (uintptr_t)hardfault_handler, // 0x0000'000C - All classes of fault
    (uintptr_t)menmanage_handler, // 0x0000'0010 - Memory management
    (uintptr_t)busfault_handler, // 0x0000'0014 - Pre-fetch fault, memory access fault
    (uintptr_t)usagefault_handler, // 0x0000'0018 - Undefined instruction or illegal state
    // subsequent interrupts left unimplemented
    // the bootloader won't be able to use these, but since the application maps its vector table
    // to SRAM, it can change interrupt handlers from the defaults at runtime
    // (see application's vector_table.c)
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
    (uintptr_t)hardfault_handler,
};
