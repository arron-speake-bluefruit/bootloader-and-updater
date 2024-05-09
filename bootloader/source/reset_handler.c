#include <stdint.h>

extern void main(void);
extern void hardfault_handler(void);

extern uint32_t _etext;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sdata;
extern uint32_t _edata;

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
