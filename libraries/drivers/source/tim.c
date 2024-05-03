#include "tim.h"
#include "critical_section.h"

// TIMx CR1 bit offsets
#define TIMx_CR1_CEN_BIT 0
#define TIMx_CR1_UDIS_BIT 1
#define TIMx_CR1_OPM_BIT 3
#define TIMx_CR1_ARPE_BIT 7

// TIMx DIER bit offsets
#define TIMx_DIER_UIE_BIT 0
#define TIMx_DIER_UDE_BIT 8

// TIMx SR bit offsets
#define TIMx_SR_UIF_BIT 0

static inline volatile uint16_t* get_register(tim_t base, uint32_t offset) {
    return (volatile uint16_t*)((uintptr_t)base + offset);
}

void tim_enable_autoreload_preload(tim_t tim) {
    volatile uint16_t* cr1 = get_register(tim, TIMx_CR1_OFFSET);
    CRITICAL_SECTION_ENTER();
    *cr1 |= (1 << TIMx_CR1_ARPE_BIT);
    CRITICAL_SECTION_EXIT();
}

void tim_enable_one_pulse_mode(tim_t tim) {
    volatile uint16_t* cr1 = get_register(tim, TIMx_CR1_OFFSET);
    CRITICAL_SECTION_ENTER();
    *cr1 |= (1 << TIMx_CR1_OPM_BIT);
    CRITICAL_SECTION_EXIT();
}

bool tim_is_enabled(tim_t tim) {
    volatile uint16_t* cr1 = get_register(tim, TIMx_CR1_OFFSET);
    return (*cr1 & (1 << TIMx_CR1_CEN_BIT)) != 0;
}

void tim_enable(tim_t tim) {
    volatile uint16_t* cr1 = get_register(tim, TIMx_CR1_OFFSET);
    CRITICAL_SECTION_ENTER();
    *cr1 |= (1 << TIMx_CR1_CEN_BIT);
    CRITICAL_SECTION_EXIT();
}

void tim_disable(tim_t tim) {
    volatile uint16_t* cr1 = get_register(tim, TIMx_CR1_OFFSET);
    CRITICAL_SECTION_ENTER();
    *cr1 &= ~(1 << TIMx_CR1_CEN_BIT);
    CRITICAL_SECTION_EXIT();
}

void tim_enable_interrupt(tim_t tim) {
    volatile uint16_t* dier = get_register(tim, TIMx_DIER_OFFSET);
    // Only defined bits in DIER are UDE for enabling DMA request and UIE for the update interrupt.
    // DMA is unimplemented, so we can always write 0. This means we can skip the read-modify steps
    // and just perform a direct write. No critical section.
    *dier = (1 << TIMx_DIER_UIE_BIT);
}

void tim_disable_interrupt(tim_t tim) {
    volatile uint16_t* dier = get_register(tim, TIMx_DIER_OFFSET);
    *dier = 0;
}

void tim_clear_interrupt_flag(tim_t tim) {
    volatile uint16_t* sr = get_register(tim, TIMx_SR_OFFSET);
    *sr = 0;
}

void tim_set_prescaler(tim_t tim, uint16_t prescale) {
    volatile uint16_t* psc = get_register(tim, TIMx_PSC_OFFSET);
    *psc = prescale;
}

void tim_set_autoreload_value(tim_t tim, uint16_t value) {
    volatile uint16_t* arr = get_register(tim, TIMx_ARR_OFFSET);
    *arr = value;
}

void tim_generate_event(tim_t tim) {
    volatile uint16_t* egr = get_register(tim, TIMx_EGR_OFFSET);
    *egr = 1;
}

uint16_t tim_get_counter(tim_t tim) {
    volatile uint16_t* cnt = get_register(tim, TIMx_CNT_OFFSET);
    return *cnt;
}

void tim_set_counter(tim_t tim, uint16_t value) {
    volatile uint16_t* cnt = get_register(tim, TIMx_CNT_OFFSET);
    *cnt = value;
}
