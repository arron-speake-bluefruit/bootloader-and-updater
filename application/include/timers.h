#ifndef APPLICATION_TIMERS_H
#define APPLICATION_TIMERS_H

// Interface for running timer-delayed callbacks.
//
// Currently, this is a very simple implementation which supports a maximum of 1 timer.

#include <stdint.h>
#include <stdbool.h>

typedef void (*timers_calback_t)(void*);

void timers_initialize(void);

bool timers_start(uint16_t delay_ms, timers_calback_t callback, void* userdata);

#endif // APPLICATION_TIMERS_H
