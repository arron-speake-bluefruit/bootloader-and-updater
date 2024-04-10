#include "rcc.h"
#include "critical_section.h"
#include <stdint.h>

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
#define RCC_AHBENR (volatile uint32_t*)(RCC_BASE_ADDRESS + RCC_AHBENR_OFFSET)
#define RCC_APB2ENR (volatile uint32_t*)(RCC_BASE_ADDRESS + RCC_APB2ENR_OFFSET)
#define RCC_APB1ENR (volatile uint32_t*)(RCC_BASE_ADDRESS + RCC_APB1ENR_OFFSET)

// RCC APB2ENR bit names
#define RCC_APB2ENR_USART1EN_BIT 14

// RCC APB1ENR bit names
#define RCC_APB1ENR_USART2EN_BIT 17

// RCC AHBENR bit names
#define RCC_AHBENR_IOPAEN_BIT 17
#define RCC_AHBENR_IOPCEN_BIT 19

void rcc_ahb_iopa_en(void) {
    CRITICAL_SECTION_ENTER();
    *RCC_AHBENR |= 1 << RCC_AHBENR_IOPAEN_BIT;
    CRITICAL_SECTION_EXIT();
}

void rcc_ahb_iopc_en(void) {
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
