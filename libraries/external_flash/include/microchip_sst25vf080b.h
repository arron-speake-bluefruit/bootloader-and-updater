#ifndef DRIVERS_MICROCHIP_SST25080B_H
#define DRIVERS_MICROCHIP_SST25080B_H

// Driver for the Microchip SST25VF080B 8-Mbit SPI Serial Flash IC
//
// Written to 'own' and operate on SPI1 using default pins PA4, PA5, PA6 and PA7.
//
// This expects an external pull-up resistor on the NSS line, and will not function without it.

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

enum {
    // Expected first byte of reported JEDEC ID, identifies Microchip as a manufacturer.
    microchip_jedec_manufacturer_id = 0xBF,

    // Expected second byte of reported JEDEC ID, identifies the SST25VF080B device.
    microchip_sst25vf080b_device_id_low = 0x25,

    // Expected second byte of reported JEDEC ID, identifies the SST25VF080B device.
    microchip_sst25vf080b_device_id_high = 0x8E,

    // Set if there is an ongoing write operation on-chip.
    microchip_sst25vf080b_status_busy_bit = 0,

    // The write-enable latch. Set if writes are enabled.
    microchip_sst25vf080b_status_wel_bit = 1,

    // Block-protect bit 0, describes block protection level.
    microchip_sst25vf080b_status_bp0_bit = 2,

    // Block-protect bit 1, describes block protection level.
    microchip_sst25vf080b_status_bp1_bit = 3,

    // Block-protect bit 2, describes block protection level.
    microchip_sst25vf080b_status_bp2_bit = 4,

    // Block-protect bit 3, describes block protection level.
    microchip_sst25vf080b_status_bp3_bit = 5,

    // Auto address increment bit, indicates the chip being in AAI-mode.
    microchip_sst25vf080b_status_aai_bit = 6,

    // Block-protect latch bit, block-protect bits can be modified only if this is set.
    microchip_sst25vf080b_status_bpl_bit = 7,
};

// Represents the JEDEC ID reported by the Microchip SST25VF080B.
typedef struct jedec_id {
    // Reported first byte, expected to equal `microchip_jedec_manufacturer_id`.
    uint8_t manufacturer;

    // Reported second byte, expected to equal `microchip_sst25vf080b_device_id_low`.
    uint8_t device_low;

    // Reported third byte, expected to equal `microchip_sst25vf080b_device_id_high`.
    uint8_t device_high;
} jedec_id_t;

// Configure SPI1's power, clocks and settings for proper operation.
void microchip_sst25vf080b_initialize(void);

// Retrieve manufactuer and device IDs from the chip. Used for checking that communication with
// the chip is working.
jedec_id_t microchip_sst25vf080b_jedec_id(void);

// Sets the write-enable latch bit on the chip. This must be called before any write/erase chip
// function. Executing a write/erase operation clears this bit, so a write-disable function is
// not required.
void microchip_sst25vf080b_write_enable(void);

// Return the chip's internal status register. See the `microchip_sst25vf080b_status_*` values
// defined in the header for specific bit information.
uint8_t microchip_sst25vf080b_read_status_register(void);

// Enables write-status-register operations to modify the chip's status register.
void microchip_sst25vf080b_enable_write_status_register(void);

// Update writeable bits in the chip's write status register. See the
// `microchip_sst25vf080b_status_*` values defined in the header for specific bit information.
void microchip_sst25vf080b_write_status_register(uint8_t value);

// Erase the aligned 4KiB sector of flash which contains `address`.
void microchip_sst25vf080b_erase_4k_sector(uint32_t address);

// Program a single byte into an address on the chip.
void microchip_sst25vf080b_byte_program(uint32_t address, uint8_t data);

// Read `size` bytes from the chip into `data`, starting at `address`.
void microchip_sst25vf080b_read(uint32_t address, size_t size, uint8_t *data);

#endif // DRIVERS_MICROCHIP_SST25080B_H
