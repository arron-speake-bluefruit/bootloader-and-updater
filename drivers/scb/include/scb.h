#ifndef DRIVERS_SCB_H
#define DRIVERS_SCB_H

#include <stdint.h>

#define SYSCFG_AIRCR_VECTKEY ((uint16_t)0x05FA)

// Asserts a signal to the system that requests a reset.
extern void scb_request_system_reset(uint16_t vectkey);

#endif // DRIVERS_SCB_H
