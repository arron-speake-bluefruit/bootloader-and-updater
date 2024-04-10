#include "scb.h"

// Peripheral base address
#define SCB_BASE_ADDRESS ((uintptr_t)0xE000ED00)

// CSB register offsets
#define SCB_CPUID_OFFSET ((uintptr_t)0x00)
#define SCB_ICSR_OFFSET ((uintptr_t)0x04)
#define SCB_AIRCR_OFFSET ((uintptr_t)0x0C)
#define SCB_SCR_OFFSET ((uintptr_t)0x10)
#define SCB_CCR_OFFSET ((uintptr_t)0x14)
#define SCB_SHPR2_OFFSET ((uintptr_t)0x1C)
#define SCB_SHPR3_OFFSET ((uintptr_t)0x20)

// CSB register pointers
#define SCB_AIRCR ((volatile uint32_t*)(SCB_BASE_ADDRESS + SCB_AIRCR_OFFSET))

// CSB AIRCR field offsets
#define SCB_AIRCR_VECTKEY_OFFSET 16
#define SCB_AIRCR_SYSRESETREQ_BIT 2

extern void scb_request_system_reset(uint16_t vectkey) {
    *SCB_AIRCR = ((uint32_t)vectkey << SCB_AIRCR_VECTKEY_OFFSET) | (1 << SCB_AIRCR_SYSRESETREQ_BIT);
}
