#include "test_assert.h"
#include "ringbuffer.h"

static void can_push_and_pop_bytes_from_ringbuffer(void) {
    // Given
    uint8_t buffer[8];
    ringbuffer_t ringbuffer = ringbuffer_initialize(buffer, sizeof(buffer));

    // When
    uint8_t byte_in = 0xAA;
    bool pushed = ringbuffer_push(&ringbuffer, byte_in);

    // Then
    ASSERT_TRUE(pushed);

    // When
    uint8_t byte_out = 0x00;
    bool popped = ringbuffer_pop(&ringbuffer, &byte_out);

    // Then
    ASSERT_TRUE(popped);
    ASSERT_EQUAL(byte_out, byte_in);

    // When
    bool popped_again = ringbuffer_pop(&ringbuffer, &byte_out);
    ASSERT_FALSE(popped_again);
}

static void cannot_push_to_full_buffer(void) {
    // Given
    uint8_t buffer[8];
    ringbuffer_t ringbuffer = ringbuffer_initialize(buffer, sizeof(buffer));

    for (size_t i = 0; i < sizeof(buffer) - 1; i++) {
        // When
        bool pushed = ringbuffer_push(&ringbuffer, (uint8_t)i);

        // Then
        ASSERT_TRUE(pushed);
    }

    // When
    bool pushed = ringbuffer_push(&ringbuffer, 0xFF);

    // Then
    ASSERT_FALSE(pushed);
}

static void buffer_can_wrap_around(void) {
    // Given
    uint8_t buffer[8];
    ringbuffer_t ringbuffer = ringbuffer_initialize(buffer, sizeof(buffer));

    // fill the buffer halfway
    for (size_t i = 0; i < sizeof(buffer) / 2; i++) {
        bool pushed = ringbuffer_push(&ringbuffer, (uint8_t)i);
        ASSERT_TRUE(pushed);
    }

    // empty it back out
    uint8_t out;
    while (ringbuffer_pop(&ringbuffer, &out)) {}

    // buffer can still be filled
    for (size_t i = 0; i < sizeof(buffer) - 1; i++) {
        // When
        bool pushed = ringbuffer_push(&ringbuffer, (uint8_t)i);

        // Then
        ASSERT_TRUE(pushed);
    }

    // When
    bool pushed = ringbuffer_push(&ringbuffer, 0xFF);

    // Then still has proper capacity check
    ASSERT_FALSE(pushed);
}

int main(void) {
    can_push_and_pop_bytes_from_ringbuffer();
    cannot_push_to_full_buffer();
    buffer_can_wrap_around();
}
