#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "event.h"
#include <stdbool.h>

void event_queue_initialize(void);

bool event_queue_push(const event_t* event);

bool event_queue_pop(event_t* event);

#endif // EVENT_QUEUE_H
