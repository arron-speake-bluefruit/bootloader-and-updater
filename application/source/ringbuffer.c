#include "ringbuffer.h"

static size_t next_index(size_t index, size_t capacity) {
    size_t next = index + 1;
    if (next >= capacity) {
        return 0;
    } else {
        return next;
    }
}

ringbuffer_t ringbuffer_initialize(uint8_t* buffer, size_t capacity) {
    return (ringbuffer_t){
        .buffer = buffer,
        .capacity = capacity,
        .write_index = 0,
        .read_index = 0,
    };
}

bool ringbuffer_push(ringbuffer_t* ringbuffer, uint8_t byte) {
    size_t next_write_index = next_index(ringbuffer->write_index, ringbuffer->capacity);
    bool is_full = (next_write_index == ringbuffer->read_index);

    if (is_full) {
        return false;
    } else {
        ringbuffer->buffer[ringbuffer->write_index] = byte;
        ringbuffer->write_index = next_write_index;
        return true;
    }
}

bool ringbuffer_pop(ringbuffer_t* ringbuffer, uint8_t* byte) {
    bool is_empty = (ringbuffer->read_index == ringbuffer->write_index);

    if (is_empty) {
        return false;
    } else {
        size_t next_read_index = next_index(ringbuffer->read_index, ringbuffer->capacity);
        *byte = ringbuffer->buffer[ringbuffer->read_index];
        ringbuffer->read_index = next_read_index;
        return true;
    }
}
