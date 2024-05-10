#include "info_region.h"
#include "flash_copy.h"
#include <stdint.h>

extern uint32_t INFO_REGION_ORIGIN;

static void erase_region(void) {
    flash_erase_pages((uint32_t)&INFO_REGION_ORIGIN, 1);
}

static void set_update_flag(void) {
    uint16_t flag = 0x0000;
    flash_copy((uint16_t*)&INFO_REGION_ORIGIN, &flag, 1);
}

void info_region_set_status(info_status_t status) {
    switch (status) {
        case info_status_no_update:
            erase_region();
            break;
        case info_status_update_ready:
            erase_region();
            set_update_flag();
            break;
    }
}

info_status_t info_region_get_status(void) {
    uint16_t flag = *(uint16_t*)&INFO_REGION_ORIGIN;

    if (flag == 0xFFFF) {
        return info_status_no_update;
    } else {
        return info_status_update_ready;
    }
}
