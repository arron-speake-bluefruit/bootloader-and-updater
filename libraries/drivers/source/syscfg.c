#include "syscfg.h"
#include "critical_section.h"
#include "driver_addresses.h"

#define SYSCFG_CFGR1 ((volatile uint32_t*)(SYSCFG_BASE_ADDRESS + SYSCFG_CFGR1_OFFSET))

#define SYSCFG_CFGR1_MEM_MODE_MASK ((uint32_t)0x3)

void syscfg_set_mem_mode(syscfg_mem_mode_t mode) {
    uint32_t bits = mode & SYSCFG_CFGR1_MEM_MODE_MASK;

    CRITICAL_SECTION_ENTER();
    uint32_t value = *SYSCFG_CFGR1;
    value &= ~SYSCFG_CFGR1_MEM_MODE_MASK;
    value |= bits;
    *SYSCFG_CFGR1 = value;
    CRITICAL_SECTION_EXIT();
}
