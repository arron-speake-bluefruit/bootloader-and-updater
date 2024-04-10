#include "usart.h"
#include "critical_section.h"

// USART register offsets
#define USART_CR1_OFFSET ((uintptr_t)0x00)
#define USART_CR2_OFFSET ((uintptr_t)0x04)
#define USART_CR3_OFFSET ((uintptr_t)0x08)
#define USART_BRR_OFFSET ((uintptr_t)0x0C)
#define USART_GTPR_OFFSET ((uintptr_t)0x10)
#define USART_RTOR_OFFSET ((uintptr_t)0x14)
#define USART_RQR_OFFSET ((uintptr_t)0x18)
#define USART_ISR_OFFSET ((uintptr_t)0x1C)
#define USART_ICR_OFFSET ((uintptr_t)0x20)
#define USART_RDR_OFFSET ((uintptr_t)0x24)
#define USART_TDR_OFFSET ((uintptr_t)0x28)

// USART CR1 bit names
#define USART_CR1_UE_BIT 0
#define USART_CR1_RE_BIT 2
#define USART_CR1_TE_BIT 3

// USART ISR bit names
#define USART_ISR_TXE_BIT 7

static volatile uint32_t* get_register(usart_t usart, uintptr_t register_offset) {
    return (volatile uint32_t*)((uintptr_t)usart + register_offset);
}

void usart_set_usartdiv(usart_t usart, uint16_t usartdiv) {
    // Direct write, no critical section needed.

    // This isn't correct for USARTs with oversampling disabled, but this behaviour isn't exposed
    // to driver callers.

    volatile uint32_t* brr = get_register(usart, USART_BRR_OFFSET);
    *brr = (uint32_t)usartdiv;
}

void usart_enable(usart_t usart) {
    volatile uint32_t* cr1 = get_register(usart, USART_CR1_OFFSET);

    CRITICAL_SECTION_ENTER();
    uint32_t value = *cr1;
    value |= (1 << USART_CR1_UE_BIT) | (1 << USART_CR1_RE_BIT) | (1 << USART_CR1_TE_BIT);
    *cr1 = value;
    CRITICAL_SECTION_EXIT();
}

bool usart_transmit_register_is_empty(usart_t usart) {
    volatile uint32_t* isr = get_register(usart, USART_ISR_OFFSET);
    return ((*isr >> USART_ISR_TXE_BIT) & 1) == 1;
}

void usart_write(usart_t usart, uint8_t byte) {
    // Direct write, no critical section needed.

    volatile uint32_t* tdr = get_register(usart, USART_TDR_OFFSET);

    *tdr = (uint32_t)byte;
}
