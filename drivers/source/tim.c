#include "tim.h"
#include "critical_section.h"

// Register offsets (TIM6 & TIM7)
#define TIMx_CR1_OFFSET ((uintptr_t)0x00)
#define TIMx_CR2_OFFSET ((uintptr_t)0x04)
#define TIMx_DIER_OFFSET ((uintptr_t)0x0C)
#define TIMx_SR_OFFSET ((uintptr_t)0x10)
#define TIMx_EGR_OFFSET ((uintptr_t)0x14)
#define TIMx_CNT_OFFSET ((uintptr_t)0x24)
#define TIMx_PSC_OFFSET ((uintptr_t)0x28)
#define TIMx_ARR_OFFSET ((uintptr_t)0x2C)

// TIMx CR1 bit offsets 
#define TIMx_CR1_CEN_BIT 0

// TIMx DIER bit offsets
#define TIMx_DIER_UIE_BIT 0
#define TIMx_DIER_UDE_BIT 8

// TIMx SR bit offsets
#define TIMx_SR_UIF_BIT 0

static inline volatile uint16_t* get_register(tim_t base, uint32_t offset) {
    return (volatile uint16_t*)((uintptr_t)base + offset);
}

void tim_enable(tim_t tim) {
    volatile uint16_t* cr1 = get_register(tim, TIMx_CR1_OFFSET);
    CRITICAL_SECTION_ENTER();
    *cr1 |= (1 << TIMx_CR1_CEN_BIT);
    CRITICAL_SECTION_EXIT();
}

void tim_enable_interrupt(tim_t tim) {
    volatile uint16_t* dier = get_register(tim, TIMx_DIER_OFFSET);
    // Only defined bits in DIER are UDE for enabling DMA request and UIE for the update interval.
    // DMA is unimplemented, so we can always write 0. This means we can skip the read-modify steps
    // and just perform a direct write. No critical section.
    *dier = (1 << TIMx_DIER_UIE_BIT);
}

void tim_clear_interrupt_flag(tim_t tim) {
    volatile uint16_t* sr = get_register(tim, TIMx_SR_OFFSET);
    // Only bit present in the register, so a direct write is used.
    *sr = (1 << TIMx_SR_UIF_BIT);
}

void tim_set_prescaler(tim_t tim, uint16_t prescale) {
    volatile uint16_t* psc = get_register(tim, TIMx_PSC_OFFSET);
    *psc = value;
}

void tim_set_autoreload_value(tim_t tim, uint16_t value) {
    volatile uint16_t* arr = get_register(tim, TIMx_ARR_OFFSET);
    *arr = value;
}
