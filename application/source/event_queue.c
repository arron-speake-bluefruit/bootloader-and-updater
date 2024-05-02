#include "event_queue.h"
#include <stddef.h>

enum {
    buffer_capacity = 16,
};

static event_t buffer[buffer_capacity];
static size_t write_index;
static size_t read_index;

void event_queue_initialize(void) {
    write_index = 0;
    read_index = 0;
}

bool event_queue_push(const event_t* event) {
    size_t next_write_index = (write_index + 1) % buffer_capacity;
    bool is_buffer_full = (next_write_index == read_index);

    if (is_buffer_full) {
        return false;
    } else {
        buffer[write_index] = *event;
        write_index = next_write_index;
        return true;
    }
}

bool event_queue_pop(event_t* event) {
    bool has_event = (write_index != read_index);

    if (has_event) {
        *event = buffer[read_index];
        read_index = (read_index + 1) % buffer_capacity;
        return true;
    } else {
        return false;
    }
}
