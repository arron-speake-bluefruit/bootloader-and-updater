#include "flash.h"
#include "critical_section.h"

// Peripheral base address
#define FLASH_BASE_ADDRESS ((uintptr_t)0x40022000)

// Peripheral register offsets
#define FLASH_ACR_OFFSET 0x00
#define FLASH_KEYR_OFFSET 0x04
#define FLASH_OPTKEYR_OFFSET 0x08
#define FLASH_SR_OFFSET 0x0C
#define FLASH_CR_OFFSET 0x10
#define FLASH_AR_OFFSET 0x14
#define FLASH_OBR_OFFSET 0x1C
#define FLASH_WRPR_OFFSET 0x20

// FLASH ACR bit names
#define FLASH_ACR_PRFTBE_BIT 4

// FLASH CR fields
#define FLASH_CR_STRT_BIT 6
#define FLASH_CR_LOCK_BIT 7

// FLASH SR bit names
#define FLASH_SR_BSY_BIT 0

// Peripheral register pointers
#define FLASH_ACR ((volatile uint32_t*)(FLASH_BASE_ADDRESS + FLASH_ACR_OFFSET))
#define FLASH_KEYR ((volatile uint32_t*)(FLASH_BASE_ADDRESS + FLASH_KEYR_OFFSET))
#define FLASH_CR ((volatile uint32_t*)(FLASH_BASE_ADDRESS + FLASH_CR_OFFSET))
#define FLASH_SR ((volatile uint32_t*)(FLASH_BASE_ADDRESS + FLASH_SR_OFFSET))
#define FLASH_AR ((volatile uint32_t*)(FLASH_BASE_ADDRESS + FLASH_AR_OFFSET))

void flash_enable_prefetch(void) {
    CRITICAL_SECTION_ENTER();
    *FLASH_ACR |= 1 << FLASH_ACR_PRFTBE_BIT;
    CRITICAL_SECTION_EXIT();
}

void flash_set_latency(flash_latency_t latency) {
    uint32_t mask = 0x00000007;
    uint32_t bits = latency & mask;

    CRITICAL_SECTION_ENTER();
    uint32_t acr = *FLASH_ACR;
    acr &= ~mask;
    acr |= bits;
    *FLASH_ACR = acr;
    CRITICAL_SECTION_EXIT();
}

void flash_lock(void) {
    CRITICAL_SECTION_ENTER();
    *FLASH_CR |= 1 << FLASH_CR_LOCK_BIT;
    CRITICAL_SECTION_EXIT();
}

void flash_unlock(uint32_t key1, uint32_t key2) {
    *FLASH_KEYR = key1;
    *FLASH_KEYR = key2;
}

bool flash_is_busy(void) {
    return ((*FLASH_SR >> FLASH_SR_BSY_BIT) & 1) == 1;
}

void flash_set_address(uint32_t address) {
    *FLASH_AR = address;
}

void flash_set_operation(flash_operation_t operation) {
    uint32_t mask = 0x00000007;
    uint32_t bits = (1 << operation);

    CRITICAL_SECTION_ENTER();
    uint32_t cr = *FLASH_CR;
    cr &= ~mask;
    cr |= bits;
    *FLASH_CR = cr;
    CRITICAL_SECTION_EXIT();
}

void flash_operation_start(void) {
    CRITICAL_SECTION_ENTER();
    *FLASH_CR |= 1 << FLASH_CR_STRT_BIT;
    CRITICAL_SECTION_EXIT();
}

uint32_t flash_status(void) {
    return *FLASH_SR;
}

void clear_flash_status(void) {
    *FLASH_SR = flash_status_bit_programming_error
        | flash_status_bit_write_protect_error
        | flash_status_bit_end_of_program;
}
