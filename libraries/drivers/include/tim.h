#ifndef DRIVERS_TIM_H
#define DRIVERS_TIM_H

// TIM7 is not exposed, since it is not available on STM32F05xx devices.

#include "driver_addresses.h"
#include <stdbool.h>

typedef enum tim {
    tim6 = TIM6_BASE_ADDRESS,
} tim_t;

// Enable TIMx_ARR register buffering.
void tim_enable_autoreload_preload(tim_t tim);

// When one-pulse mode is enabled, the counter stops counting when an event is generated,
// instead of repeating.
void tim_enable_one_pulse_mode(tim_t tim);

// Returns whether the timer's CR1_CEN bit is set.
bool tim_is_enabled(tim_t tim);

// Enable the given timer peripheral's counter.
void tim_enable(tim_t tim);

// Disable the given timer peripheral's counter.
void tim_disable(tim_t tim);

// Enable the update event interrupt, triggered when the given timer overflows.
void tim_enable_interrupt(tim_t tim);

// Disable the update event interrupt for the given timer.
void tim_disable_interrupt(tim_t tim);

// Clear the interrupt flag, asserted by the timer overflowing, or when explicitly reinitialised.
void tim_clear_interrupt_flag(tim_t tim);

// Set the prescaler value used to divide the given timer's clock source.
void tim_set_prescaler(tim_t tim, uint16_t prescale);

// Set the auto-reload value of the given timer.
void tim_set_autoreload_value(tim_t tim, uint16_t value);

// Re-initialize the timer counter and generate an update of its registers. The prescaler counter
// is cleared (but the prescaler ratio is not affected)
void tim_generate_event(tim_t tim);

// Get the current counter of the given timer. Basic timers are always upcounting, so this value
// increases towards the reload value.
uint16_t tim_get_counter(tim_t tim);

// Set the current counter of the given timer.
void tim_set_counter(tim_t tim, uint16_t value);

#endif // DRIVERS_TIM_H
