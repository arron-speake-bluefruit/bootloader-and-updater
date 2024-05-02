#include "timers.h"
#include "rcc.h"
#include "vector_table.h"
#include "nvic.h"
#include "tim.h"

static timers_calback_t timer_callback;
static void* timer_userdata;

static void tim6_global_interrupt(void) {
    tim_clear_interrupt_flag(tim6);
    timer_callback(timer_userdata);
}

void timers_initialize(void) {
    // Power on the TIM6 peripheral
    rcc_apb1_tim6_enable();

    // Configure the TIM6's interrupt
    vector_table_set(vector_table_index_tim6, tim6_global_interrupt);
    nvic_enable_tim6_global_interrupt();
    tim_enable_interrupt(tim6);

    // 48MHz system clock / 48000 prescaler = 1kHz timer => counts in milliseconds
    tim_set_autoreload_value(tim6, UINT16_MAX);
    tim_set_prescaler(tim6, 48000);
    tim_enable_autoreload_preload(tim6);
    tim_enable_one_pulse_mode(tim6);
}

bool timers_start(uint16_t delay_ms, timers_calback_t callback, void* userdata) {
    // if a timer is already running, don't start a new one
    if (tim_is_enabled(tim6)) {
        return false;
    }

    timer_callback = callback;
    timer_userdata = userdata;
    tim_set_autoreload_value(tim6, delay_ms);
    tim_enable(tim6);
    return true;
}
