#include "scb.h"
#include "driver_addresses.h"

// CSB register pointers
#define SCB_AIRCR ((volatile uint32_t*)(SCB_AIRCR_ADDRESS))

// CSB AIRCR field offsets
#define SCB_AIRCR_VECTKEY_OFFSET 16
#define SCB_AIRCR_SYSRESETREQ_BIT 2

extern void scb_request_system_reset(uint16_t vectkey) {
    *SCB_AIRCR = ((uint32_t)vectkey << SCB_AIRCR_VECTKEY_OFFSET) | (1 << SCB_AIRCR_SYSRESETREQ_BIT);
}
