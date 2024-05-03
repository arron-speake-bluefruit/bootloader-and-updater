#include "nvic.h"
#include "driver_addresses.h"
#include <stddef.h>

#define NVIC_ISER ((volatile uint32_t*)NVIC_ISER_ADDRESS)
#define NVIC_ICER ((volatile uint32_t*)NVIC_ICER_ADDRESS)

#define NVIC_ISER_USART2_BIT 28
#define NVIC_ISER_TIM6_BIT 17

void nvic_enable_usart2_global_interrupt(void) {
    *NVIC_ISER = 1 << NVIC_ISER_USART2_BIT;
}

void nvic_enable_tim6_global_interrupt(void) {
    *NVIC_ISER = 1 << NVIC_ISER_TIM6_BIT;
}

void nvic_disable_tim6_global_interrupt(void) {
    *NVIC_ICER = 1 << NVIC_ISER_TIM6_BIT;
}
