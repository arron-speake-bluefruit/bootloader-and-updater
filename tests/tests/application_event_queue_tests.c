#include "test_assert.h"
#include "event_queue.h"

static void empty_queue_cannot_pop_event(void) {
    // Given
    event_queue_initialize();

    // When
    event_t event;
    bool popped = event_queue_pop(&event);

    // Then
    ASSERT_FALSE(popped);
}

static void can_push_and_pop_single_event(void) {
    // Given
    event_queue_initialize();

    // When
    event_t event_in = {
        .type = event_type_usart_rx,
        .usart_rx = 'a',
    };
    bool pushed = event_queue_push(&event_in);

    // Then
    ASSERT_TRUE(pushed);

    // When
    event_t event_out;
    bool popped = event_queue_pop(&event_out);

    // Then
    ASSERT_TRUE(popped);
    ASSERT_EQUAL(event_out.type, event_in.type);
    ASSERT_EQUAL(event_out.usart_rx, event_in.usart_rx);
}

static void can_pop_multiple_pushed_events_from_queue(void) {
    // Given
    event_queue_initialize();

    // And
    const event_t events_in[] = {
        {
            .type = event_type_usart_rx,
            .usart_rx = 'a',
        },
        {
            .type = event_type_usart_rx,
            .usart_rx = 'b',
        },
        {
            .type = event_type_usart_rx,
            .usart_rx = 'c',
        },
    };
    const size_t events_in_count = sizeof(events_in) / sizeof(events_in[0]);
    for (size_t i = 0; i < events_in_count; i++) {
        bool pushed = event_queue_push(&events_in[i]);
        ASSERT_TRUE(pushed);
    }

    for (size_t i = 0; i < events_in_count; i++) {
        // When
        event_t event_out;
        bool popped = event_queue_pop(&event_out);

        // Then
        ASSERT_TRUE(popped);
        ASSERT_EQUAL(event_out.type, events_in[i].type);
        ASSERT_EQUAL(event_out.usart_rx, events_in[i].usart_rx);
    }

    // When
    event_t event_out;
    bool popped = event_queue_pop(&event_out);

    // Then
    ASSERT_FALSE(popped);
}

static void exceeding_capacity_causes_push_to_fail(void) {
    // Given
    event_queue_initialize();
    const size_t capacity = 4; // If the internal EQ definition changes, this will need to match it

    for (size_t i = 0; i < (capacity - 1); i++) {
        event_t event = {0};
        // When
        bool pushed = event_queue_push(&event);

        // Then
        ASSERT_TRUE(pushed);
    }

    // When
    event_t event = {0};
    bool pushed = event_queue_push(&event);

    // Then
    ASSERT_FALSE(pushed);
}

int main(void) {
    empty_queue_cannot_pop_event();
    can_push_and_pop_single_event();
    can_pop_multiple_pushed_events_from_queue();
    exceeding_capacity_causes_push_to_fail();
}
