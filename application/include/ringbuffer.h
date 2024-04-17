#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct ringbuffer {
    uint8_t* buffer;
    size_t capacity;
    size_t write_index;
    size_t read_index;
} ringbuffer_t;

ringbuffer_t ringbuffer_initialize(uint8_t* buffer, size_t capacity);
bool ringbuffer_push(ringbuffer_t* ringbuffer, uint8_t byte);
bool ringbuffer_pop(ringbuffer_t* ringbuffer, uint8_t* byte);

#endif // RINGBUFFER_H
