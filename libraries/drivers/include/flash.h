#ifndef DRIVERS_FLASH_H
#define DRIVERS_FLASH_H

#include <stdbool.h>
#include <stdint.h>

#define FLASH_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)

typedef enum flash_operation {
    flash_operation_program = 0,
    flash_operation_page_erase = 1,
    flash_operation_mass_erase = 2,
} flash_operation_t;

typedef enum flash_latency {
    // Zero wait states during flash accesses (SYSCLK <= 24MHz)
    flash_latency_zero = 0x0,

    // One wait state during flash accesses (SYSCLK > 24MHz)
    flash_latency_one = 0x1,
} flash_latency_t;

// Enable the prefetch buffer.
void flash_enable_prefetch(void);

// Set the ratio of the SYSCLK (system clock) period to the flash access time.
void flash_set_latency(flash_latency_t latency);

// Lock the flash, preventing write-access to the peripheral.
void flash_lock(void);

// Unlock the flash, allowing write and erase operations onto the peripheral. Incorrect key
// arguments will lock the flash until the next reset.
void flash_unlock(uint32_t key1, uint32_t key2);

// Return true if a flash operation is ongoing, false otherwise.
bool flash_is_busy(void);

// Set the target address for subsequent flash operations.
void flash_set_address(uint32_t address);

// Set the selected flash operation to execute.
void flash_set_operation(flash_operation_t operation);

// Begin a previously configured flash operation.
void flash_operation_start(void);

typedef enum flash_status_bit {
    flash_status_bit_busy = 1 << 0,
    flash_status_bit_programming_error = 1 << 2,
    flash_status_bit_write_protect_error = 1 << 4,
    flash_status_bit_end_of_program = 1 << 5,
} flash_status_bit_t;

// Return the status of the flash (see `flash_status_bit_t`).
uint32_t flash_status(void);

// Reset reported flash status.
void flash_clear_status(void);

#endif // DRIVERS_FLASH_H
