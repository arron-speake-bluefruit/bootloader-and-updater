#include "usart.h"
#include "critical_section.h"

// USART CR1 bit names
#define USART_CR1_UE_BIT 0
#define USART_CR1_RE_BIT 2
#define USART_CR1_TE_BIT 3
#define USART_CR1_IDLEIE_BIT 4
#define USART_CR1_RXNEIE_BIT 5
#define USART_CR1_TXEIE_BIT 7

// USART ISR bit names
#define USART_ISR_RXNE_BIT 5
#define USART_ISR_TC_BIT 6
#define USART_ISR_TXE_BIT 7

// USART ICR bit names
#define USART_ICD_IDLECF_BIT 4

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
    uint32_t bits = (1 << USART_CR1_UE_BIT) | (1 << USART_CR1_RE_BIT) | (1 << USART_CR1_TE_BIT);

    CRITICAL_SECTION_ENTER();
    uint32_t value = *cr1;
    value |= bits;
    *cr1 = value;
    CRITICAL_SECTION_EXIT();
}

void usart_disable(usart_t usart) {
    volatile uint32_t* cr1 = get_register(usart, USART_CR1_OFFSET);
    uint32_t bits = (1 << USART_CR1_UE_BIT) | (1 << USART_CR1_RE_BIT) | (1 << USART_CR1_TE_BIT);

    CRITICAL_SECTION_ENTER();
    uint32_t value = *cr1;
    value &= ~bits;
    *cr1 = value;
    CRITICAL_SECTION_EXIT();
}

bool usart_transmit_register_is_empty(usart_t usart) {
    volatile uint32_t* isr = get_register(usart, USART_ISR_OFFSET);
    return ((*isr >> USART_ISR_TXE_BIT) & 1) == 1;
}

bool usart_tranmission_complete(usart_t usart) {
    volatile uint32_t* isr = get_register(usart, USART_ISR_OFFSET);
    return ((*isr >> USART_ISR_TC_BIT) & 1) == 1;
}

void usart_write(usart_t usart, uint8_t byte) {
    // Direct write, no critical section needed.

    volatile uint32_t* tdr = get_register(usart, USART_TDR_OFFSET);

    *tdr = (uint32_t)byte;
}

uint8_t usart_read(usart_t usart) {
    volatile uint32_t* rdr = get_register(usart, USART_RDR_OFFSET);
    return (uint8_t)*rdr;
}

bool usart_can_read(usart_t usart) {
    volatile uint32_t* isr = get_register(usart, USART_ISR_OFFSET);
    return ((*isr >> USART_ISR_RXNE_BIT) & 1) == 1;
}

void usart_enable_receive_interrupt(usart_t usart) {
    volatile uint32_t* cr1 = get_register(usart, USART_CR1_OFFSET);

    CRITICAL_SECTION_ENTER();
    *cr1 |= 1 << USART_CR1_RXNEIE_BIT;
    CRITICAL_SECTION_EXIT();
}

void usart_enable_idle_interrupt(usart_t usart) {
    volatile uint32_t* cr1 = get_register(usart, USART_CR1_OFFSET);

    CRITICAL_SECTION_ENTER();
    *cr1 |= 1 << USART_CR1_IDLEIE_BIT;
    CRITICAL_SECTION_EXIT();
}

void usart_clear_idle_line(usart_t usart) {
    volatile uint32_t* icr = get_register(usart, USART_ICR_OFFSET);

    // Writing 0s to this register has no effect, so do a direct write.
    *icr = (1 << USART_ICD_IDLECF_BIT);
}

void usart_enable_transmit_empty_interrupt(usart_t usart) {
    volatile uint32_t* cr1 = get_register(usart, USART_CR1_OFFSET);

    CRITICAL_SECTION_ENTER();
    *cr1 |= 1 << USART_CR1_TXEIE_BIT;
    CRITICAL_SECTION_EXIT();
}

void usart_disable_transmit_empty_interrupt(usart_t usart) {
    volatile uint32_t* cr1 = get_register(usart, USART_CR1_OFFSET);

    CRITICAL_SECTION_ENTER();
    *cr1 &= ~(1 << USART_CR1_TXEIE_BIT);
    CRITICAL_SECTION_EXIT();
}
