#include "nvic.h"
#include <stddef.h>
#include <stdint.h>

#define NVIC_ISER ((volatile uint32_t*)0xE000E100)

#define NVIC_ISER_USART2_BIT 28
#define NVIC_ISER_TIM6_BIT 17

void nvic_enable_usart2_global_interrupt(void) {
    *NVIC_ISER = 1 << NVIC_ISER_USART2_BIT;
}

void nvic_enable_tim6_global_interrupt(void) {
    *NVIC_ISER = 1 << NVIC_ISER_TIM6_BIT;
}
