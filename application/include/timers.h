#ifndef APPLICATION_TIMERS_H
#define APPLICATION_TIMERS_H

// Interface for delaying events.
//
// Currently, this is a very simple implementation which supports a maximum of 1 timer.

#include "event.h"

typedef void (*timers_calback_t)(void*);

void timers_initialize(void);

bool timers_start(uint16_t delay_ms, const event_t* event);

void timers_restart(void);

void timers_cancel(void);

#endif // APPLICATION_TIMERS_H
