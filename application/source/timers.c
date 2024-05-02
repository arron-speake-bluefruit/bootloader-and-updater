#include "timers.h"
#include "rcc.h"
#include "vector_table.h"
#include "nvic.h"
#include "tim.h"
#include "event_queue.h"

static event_t timer_event;

static void tim6_global_interrupt(void) {
    tim_clear_interrupt_flag(tim6);
    tim_disable(tim6);
    event_queue_push(&timer_event);
}

void timers_initialize(void) {
    // Power on the TIM6 peripheral
    rcc_apb1_tim6_enable();

    // Configure the TIM6's interrupt
    vector_table_set(vector_table_index_tim6, tim6_global_interrupt);
    nvic_enable_tim6_global_interrupt();
    tim_enable_interrupt(tim6);

    // 48MHz system clock / 48000 prescaler = 1kHz timer => counts in milliseconds
    tim_set_prescaler(tim6, 48000);
    tim_enable_autoreload_preload(tim6);
    tim_enable_one_pulse_mode(tim6);
}

bool timers_start(uint16_t delay_ms, const event_t* event) {
    // if a timer is already running, don't start a new one
    if (tim_is_enabled(tim6)) {
        return false;
    }

    timer_event = *event;

    // to move the autoreload value into TIM6's internal register, an update event is generated.
    // To stop the update event from triggering an interrupt, it its disabled during the update.
    // TODO: There is probably a simpler way to do this.
    tim_disable_interrupt(tim6);
    tim_set_autoreload_value(tim6, delay_ms);
    tim_generate_event(tim6);
    tim_clear_interrupt_flag(tim6);
    vector_table_set(vector_table_index_tim6, tim6_global_interrupt);
    tim_enable_interrupt(tim6);

    tim_enable(tim6);
    return true;
}

void timers_restart(void) {
    tim_set_counter(tim6, 0);
}

void timers_cancel(void) {
    tim_disable(tim6);
}
