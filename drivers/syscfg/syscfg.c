#include "syscfg.h"
#include "critical_section.h"
#include <stdint.h>

#define SYSCFG_BASE_ADDRESS ((uintptr_t)0x40010000)

#define SYSCFG_CFGR1_OFFSET ((uintptr_t)0x00)
#define SYSCFG_EXTICR1_OFFSET ((uintptr_t)0x08)
#define SYSCFG_EXTICR2_OFFSET ((uintptr_t)0x0C)
#define SYSCFG_EXTICR3_OFFSET ((uintptr_t)0x10)
#define SYSCFG_EXTICR4_OFFSET ((uintptr_t)0x14)
#define SYSCFG_CFGR2_OFFSET ((uintptr_t)0x18)

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
