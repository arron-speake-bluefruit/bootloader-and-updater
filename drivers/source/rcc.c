#include "rcc.h"
#include "critical_section.h"

// Periphal base address
#define RCC_BASE_ADDRESS ((uintptr_t)0x40021000)

// Register offsets
#define RCC_CR_OFFSET ((uintptr_t)0x00)
#define RCC_CFGR_OFFSET ((uintptr_t)0x04)
#define RCC_CIR_OFFSET ((uintptr_t)0x08)
#define RCC_APB2RSTR_OFFSET ((uintptr_t)0x0C)
#define RCC_APB1RSTR_OFFSET ((uintptr_t)0x10)
#define RCC_AHBENR_OFFSET ((uintptr_t)0x14)
#define RCC_APB2ENR_OFFSET ((uintptr_t)0x18)
#define RCC_APB1ENR_OFFSET ((uintptr_t)0x1C)
#define RCC_BDCR_OFFSET ((uintptr_t)0x20)
#define RCC_CSR_OFFSET ((uintptr_t)0x24)
#define RCC_AHBRSTR_OFFSET ((uintptr_t)0x28)
#define RCC_CFGR2_OFFSET ((uintptr_t)0x2C)
#define RCC_CFGR3_OFFSET ((uintptr_t)0x30)
#define RCC_CR2_OFFSET ((uintptr_t)0x34)

// Register addresses
#define RCC_CR (volatile uint32_t*)(RCC_BASE_ADDRESS + RCC_CR_OFFSET)
#define RCC_CFGR (volatile uint32_t*)(RCC_BASE_ADDRESS + RCC_CFGR_OFFSET)
#define RCC_AHBENR (volatile uint32_t*)(RCC_BASE_ADDRESS + RCC_AHBENR_OFFSET)
#define RCC_APB2ENR (volatile uint32_t*)(RCC_BASE_ADDRESS + RCC_APB2ENR_OFFSET)
#define RCC_APB1ENR (volatile uint32_t*)(RCC_BASE_ADDRESS + RCC_APB1ENR_OFFSET)

// RCC CR bit names
#define RCC_CR_PLLRDY_BIT 25
#define RCC_CR_PLLON_BIT 24

// RCC CFGR bit names
#define RCC_CFGR_PLLMUL_OFFSET 18
#define RCC_CFGR_PLLMUL_MASK 0xF
#define RCC_CFGR_SW_OFFSET 0
#define RCC_CFGR_SW_MASK 0x3

// RCC APB2ENR bit names
#define RCC_APB2ENR_USART1EN_BIT 14

// RCC APB1ENR bit names
#define RCC_APB1ENR_TIM6EN_BIT 4
#define RCC_APB1ENR_TIM7EN_BIT 5
#define RCC_APB1ENR_USART2EN_BIT 17

// RCC AHBENR bit names
#define RCC_AHBENR_IOPAEN_BIT 17
#define RCC_AHBENR_IOPCEN_BIT 19

void rcc_set_pll_multiplication_factor(uint8_t pllmul) {
    uint32_t bits = (uint32_t)(pllmul & RCC_CFGR_PLLMUL_MASK) << RCC_CFGR_PLLMUL_OFFSET;
    uint32_t mask = (uint32_t)RCC_CFGR_PLLMUL_MASK << RCC_CFGR_PLLMUL_OFFSET;

    CRITICAL_SECTION_ENTER();
    uint32_t value = *RCC_CFGR;
    value &= ~mask;
    value |= bits;
    *RCC_CFGR = value;
    CRITICAL_SECTION_EXIT();
}

void rcc_enable_pll(void) {
    CRITICAL_SECTION_ENTER();
    *RCC_CR |= 1 << RCC_CR_PLLON_BIT;
    CRITICAL_SECTION_EXIT();
}

bool rcc_pll_clock_ready(void) {
    uint32_t cr = *RCC_CR;
    return ((cr >> RCC_CR_PLLRDY_BIT) & 1) == 1;
}

void rcc_switch_system_clock(rcc_system_clock_t clock) {
    uint32_t bits = (uint32_t)(clock & RCC_CFGR_SW_MASK) << RCC_CFGR_SW_OFFSET;
    uint32_t mask = (uint32_t)RCC_CFGR_SW_MASK << RCC_CFGR_SW_OFFSET;

    CRITICAL_SECTION_ENTER();
    uint32_t value = *RCC_CFGR;
    value &= ~mask;
    value |= bits;
    *RCC_CFGR = value;
    CRITICAL_SECTION_EXIT();
}

void rcc_ahb_iopa_enable(void) {
    CRITICAL_SECTION_ENTER();
    *RCC_AHBENR |= 1 << RCC_AHBENR_IOPAEN_BIT;
    CRITICAL_SECTION_EXIT();
}

void rcc_ahb_iopc_enable(void) {
    CRITICAL_SECTION_ENTER();
    *RCC_APB2ENR |= 1 << RCC_AHBENR_IOPCEN_BIT;
    CRITICAL_SECTION_EXIT();
}

void rcc_apb2_usart1_enable(void) {
    CRITICAL_SECTION_ENTER();
    *RCC_APB2ENR |= 1 << RCC_APB2ENR_USART1EN_BIT;
    CRITICAL_SECTION_EXIT();
}

void rcc_apb1_usart2_enable(void) {
    CRITICAL_SECTION_ENTER();
    *RCC_APB1ENR |= 1 << RCC_APB1ENR_USART2EN_BIT;
    CRITICAL_SECTION_EXIT();
}

void rcc_ahb_iopa_disable(void) {
    CRITICAL_SECTION_ENTER();
    *RCC_AHBENR &= ~(1 << RCC_AHBENR_IOPAEN_BIT);
    CRITICAL_SECTION_EXIT();
}

void rcc_apb1_usart2_disable(void) {
    CRITICAL_SECTION_ENTER();
    *RCC_APB1ENR |= ~(1 << RCC_APB1ENR_USART2EN_BIT);
    CRITICAL_SECTION_EXIT();
}

void rcc_apb1_tim6_enable(void) {
    CRITICAL_SECTION_ENTER();
    *RCC_APB1ENR |= ~(1 << RCC_APB1ENR_TIM6EN_BIT);
    CRITICAL_SECTION_EXIT();
}

void rcc_apb1_tim7_enable(void) {
    CRITICAL_SECTION_ENTER();
    *RCC_APB1ENR |= ~(1 << RCC_APB1ENR_TIM7EN_BIT);
    CRITICAL_SECTION_EXIT();
}
