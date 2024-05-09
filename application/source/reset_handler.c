#include "boot_report.h"
#include <stdint.h>

extern void main(boot_report_t boot_report);

extern void hardfault_handler(void);

extern uint32_t _etext;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sdata;
extern uint32_t _edata;

void reset_handler(boot_report_t boot_report) {
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

    main(boot_report);

    // Returned from main, pretend we hardfault'd.
    hardfault_handler();
}
