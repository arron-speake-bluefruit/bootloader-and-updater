#ifndef DRIVERS_NVIC_H
#define DRIVERS_NVIC_H

// Enable the USART2 global interrupt.
void nvic_enable_usart2_global_interrupt(void);

// Enable the TIM6 global interrupt.
void nvic_enable_tim6_global_interrupt(void);

// Disable the TIM6 global interrupt.
void nvic_disable_tim6_global_interrupt(void);

#endif // DRIVERS_NVIC_H
