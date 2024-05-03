#include <stdbool.h>
#include <stdint.h>

extern uint32_t _etext;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _estack;

extern void main(void);

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

void reset_handler(void) {
    // Copy data init values from text
    uint32_t *init_values_ptr = &_etext;
    uint32_t *data_ptr = &_sdata;
    while (data_ptr < &_edata) {
        *data_ptr = *init_values_ptr;
        data_ptr++;
        init_values_ptr++;
    }

    // Zero out the BSS.
    for (uint32_t* p = &_sbss; p < &_ebss; p++) {
        *p = 0;
    }

    main();

    // Returned from main, pretend we hardfault'd.
    hardfault_handler();
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
