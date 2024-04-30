#ifndef DRIVERS_RCC_H
#define DRIVERS_RCC_H

#include <stdint.h>
#include <stdbool.h>

typedef enum rcc_system_clock {
    rcc_system_clock_hsi = 0x0,
    rcc_system_clock_hse = 0x1,
    rcc_system_clock_pll = 0x2,
} rcc_system_clock_t;

// Set the multiplication factor of the PLL, must not cause the system clock to exceed 48MHz.
void rcc_set_pll_multiplication_factor(uint8_t pllmul);

// Enable the PLL clock.
void rcc_enable_pll(void);

// Returns whether the PLL clock is ready to be used as the system clock.
bool rcc_pll_clock_ready(void);

// Select which clock is used as the system clock.
void rcc_switch_system_clock(rcc_system_clock_t clock);

// Enable the I/O port A clock.
void rcc_ahb_iopa_enable(void);

// Enable the I/O port C clock.
void rcc_ahb_iopc_enable(void);

// Enable the USART1 clock.
void rcc_apb2_usart1_enable(void);

// Enable the USART2 clock.
void rcc_apb1_usart2_enable(void);

// Disable the I/O port A clock.
void rcc_ahb_iopa_disable(void);

// Disable the USART2 clock.
void rcc_apb1_usart2_disable(void);

// Enable TIM6 clock.
void rcc_apb1_tim6_enable(void);

// Enable TIM7 clock.
void rcc_apb1_tim7_enable(void);

#endif // DRIVERS_RCC_H
