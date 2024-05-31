#include "spi.h"
#include "driver_addresses.h"
#include "critical_section.h"

#define SPIx_CR1_MSTR_BIT 2
#define SPIx_CR1_BR_OFFSET 3
#define SPIx_CR1_BR_MASK 0x7
#define SPIx_CR1_SPE_BIT 6

#define SPIx_CR2_SSOE_BIT 2

#define SPIx_SR_BSY_OFFSET 7
#define SPIx_SR_FRLVL_OFFSET 9
#define SPIx_SR_FRLVL_MASK 0x3
#define SPIx_SR_FTLVL_OFFSET 11
#define SPIx_SR_FTLVL_MASK 0x3

static volatile uint16_t* get_register(spi_t spi, uintptr_t offset) {
    return (volatile uint16_t*)((uintptr_t)spi + offset);
}

void spi_enable(spi_t spi) {
    volatile uint16_t* cr1 = get_register(spi, SPIx_CR1_OFFSET);

    CRITICAL_SECTION_ENTER();
    *cr1 |= (1 << SPIx_CR1_SPE_BIT);
    CRITICAL_SECTION_EXIT();
}

void spi_disable(spi_t spi) {
    volatile uint16_t* cr1 = get_register(spi, SPIx_CR1_OFFSET);

    CRITICAL_SECTION_ENTER();
    *cr1 &= ~(1 << SPIx_CR1_SPE_BIT);
    CRITICAL_SECTION_EXIT();
}

void spi_transmit(spi_t spi, uint8_t data) {
    volatile uint8_t* dr = (volatile uint8_t*)get_register(spi, SPIx_DR_OFFSET);
    *dr = data;
}

uint8_t spi_receive(spi_t spi) {
    volatile uint8_t* dr = (volatile uint8_t*)get_register(spi, SPIx_DR_OFFSET);
    return *dr;
}

spi_fifo_level_t spi_get_tx_fifo_level(spi_t spi) {
    volatile uint16_t* sr = get_register(spi, SPIx_SR_OFFSET);

    return (*sr >> SPIx_SR_FTLVL_OFFSET) & SPIx_SR_FTLVL_MASK;
}

spi_fifo_level_t spi_get_rx_fifo_level(spi_t spi) {
    volatile uint16_t* sr = get_register(spi, SPIx_SR_OFFSET);

    return (*sr >> SPIx_SR_FRLVL_OFFSET) & SPIx_SR_FRLVL_MASK;
}

bool spi_is_busy(spi_t spi) {
    volatile uint16_t* sr = get_register(spi, SPIx_SR_OFFSET);

    return ((*sr >> SPIx_SR_BSY_OFFSET) & 1) == 1;
}

void spi_set_baud_rate_control(spi_t spi, uint8_t n) {
    volatile uint16_t* cr1 = get_register(spi, SPIx_CR1_OFFSET);
    uint16_t mask = SPIx_CR1_BR_MASK << SPIx_CR1_BR_OFFSET;
    uint16_t bits = (n << SPIx_CR1_BR_OFFSET) & mask;

    CRITICAL_SECTION_ENTER();
    uint16_t value = *cr1;
    value &= ~mask;
    value |= bits;
    *cr1 = value;
    CRITICAL_SECTION_EXIT();
}

void spi_set_master_mode(spi_t spi) {
    volatile uint16_t* cr1 = get_register(spi, SPIx_CR1_OFFSET);

    CRITICAL_SECTION_ENTER();
    *cr1 |= (1 << SPIx_CR1_MSTR_BIT);
    CRITICAL_SECTION_EXIT();
}

void spi_enable_ss_output(spi_t spi) {
    volatile uint16_t* cr2 = get_register(spi, SPIx_CR2_OFFSET);

    CRITICAL_SECTION_ENTER();
    *cr2 |= (1 << SPIx_CR2_SSOE_BIT);
    CRITICAL_SECTION_EXIT();
}
