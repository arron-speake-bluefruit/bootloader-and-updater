#ifndef DRIVERS_EXTERNAL_FLASH_H
#define DRIVERS_EXTERNAL_FLASH_H

// Higher-level abstraction over the Microchip SST25VF080B driver.

#include <stdbool.h>
#include <stdint.h>

// Initialize and test the external flash.
bool external_flash_initialize(void);

// Disable on-chip write/erase protection. (This is different to write _enable_, which is handled
// by write and erase functions internally).
void external_flash_unprotect(void);

// Enable on-chip write/erase protection. (This is different to write _enable_, which is handled
// by write and erase functions internally).
void external_flash_protect(void);

// Erase `sector_count` 4KiB sectors starting at the sector containing `sector_address`.
void external_flash_erase_sectors(uint32_t sector_address, uint32_t sector_count);

// Starting at `address`, read `size` bytes of data from external flash into `data`.
void external_flash_read(uint32_t address, uint32_t size, void* out);

// Write `size` bytes from `data` into external flash, starting at `address`. Must be written into
// previously erased sectors.
void external_flash_write(uint32_t address, uint32_t size, const void* data);

#endif // DRIVERS_EXTERNAL_FLASH_H
