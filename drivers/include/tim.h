#ifndef DRIVERS_TIM_H
#define DRIVERS_TIM_H

// TIM7 is not exposed, since it is not available on STM32F05 devices.

#include <stdint.h>

typedef enum tim {
    // tim17 = ((uintptr_t)0x40014800),
    // tim16 = ((uintptr_t)0x40014400),
    // tim15 = ((uintptr_t)0x40014000),
    // tim1 = ((uintptr_t)0x40012C00),
    // tim14 = ((uintptr_t)0x40002000),
    tim6 = ((uintptr_t)0x40001000),
    // tim3 = ((uintptr_t)0x40000400),
    // tim2 = ((uintptr_t)0x40000000),
} tim_t;

// Enable the given timer peripheral's counter and auto-reload preload buffer.
void tim_enable(tim_t tim);

// Enable the update event interrupt, triggered when the given timer overflows.
void tim_enable_interrupt(tim_t tim);

// Clear the interrupt flag, asserted by the timer overflowing, or when explicitly reinitialised.
void tim_clear_interrupt_flag(tim_t tim);

// Set the prescaler value used to divide the given timer's clock source.
void tim_set_prescaler(tim_t tim, uint16_t prescale);

// Set the auto-reload value of the given timer.
void tim_set_autoreload_value(tim_t tim, uint16_t value);

// Re-initialize the timer counter and generate an update of its registers. The prescaler counter
// is cleared (but the prescaler ratio is not affected)
void tim_generate_event(tim_t tim);

#endif // DRIVERS_TIM_H
