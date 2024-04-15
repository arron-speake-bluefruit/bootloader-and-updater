#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

typedef enum event_type {
    event_type_usart_rx,
} event_type_t;

typedef struct event {
    event_type_t type;
    union {
        uint8_t usart_rx;
    };
} event_t;

#endif // EVENT_H
