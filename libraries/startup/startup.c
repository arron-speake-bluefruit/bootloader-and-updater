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
    (uintptr_t)hardfault_handler, // 0x0000'001C - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0020 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0024 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0028 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'002C - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0030 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0034 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0038 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'003C - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0040 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0044 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0048 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'004C - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0050 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0054 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0058 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'005C - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0060 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0064 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0068 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'006C - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0070 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0074 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0078 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'007C - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0080 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0084 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0088 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'008C - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0090 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0094 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'0098 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'009C - TODO
    (uintptr_t)hardfault_handler, // 0x0000'00A0 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'00A4 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'00A8 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'00AC - TODO
    (uintptr_t)hardfault_handler, // 0x0000'00B0 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'00B4 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'00B8 - TODO
    (uintptr_t)hardfault_handler, // 0x0000'00BC - TODO
};
