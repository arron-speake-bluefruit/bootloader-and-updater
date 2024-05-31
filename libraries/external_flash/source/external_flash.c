#include "external_flash.h"
#include "microchip_sst25vf080b.h"

static bool check_chip_id(void) {
    jedec_id_t id = microchip_sst25vf080b_jedec_id();

    return (id.manufacturer == microchip_jedec_manufacturer_id)
        && (id.device_low == microchip_sst25vf080b_device_id_low)
        && (id.device_high == microchip_sst25vf080b_device_id_high);
}

static bool is_busy(void) {
    uint8_t status = microchip_sst25vf080b_read_status_register();
    return (status & microchip_sst25vf080b_status_busy_bit) != 0;
}

static void wait_until_not_busy(void) {
    while (is_busy()) {}
}

static void erase_sector(uint32_t address) {
    microchip_sst25vf080b_write_enable();
    microchip_sst25vf080b_erase_4k_sector(address);
    wait_until_not_busy();
}

bool external_flash_initialize(void) {
    microchip_sst25vf080b_initialize();
    return check_chip_id();
}

void external_flash_unprotect(void) {
    // Before disabling block protection, enable writes to the block-protect bits in the
    // status register.
    uint8_t first_write = 1 << microchip_sst25vf080b_status_bpl_bit;
    microchip_sst25vf080b_enable_write_status_register();
    microchip_sst25vf080b_write_status_register(first_write);

    // Write all zeros, disabling block protection and disabling writing to the block-protect bits.
    uint8_t second_write = 0x00;
    microchip_sst25vf080b_enable_write_status_register();
    microchip_sst25vf080b_write_status_register(second_write);
}

void external_flash_protect(void) {
    // Before enabling block protection, enable writes to the block-protect bits in the
    // status register.
    uint8_t first_write = 1 << microchip_sst25vf080b_status_bpl_bit;
    microchip_sst25vf080b_enable_write_status_register();
    microchip_sst25vf080b_write_status_register(first_write);

    // Enable block-protect bits 0, 1 and 2. This protects the entire memory range.
    uint8_t second_write = (1 << microchip_sst25vf080b_status_bp0_bit)
        | (1 << microchip_sst25vf080b_status_bp1_bit)
        | (1 << microchip_sst25vf080b_status_bp2_bit);
    microchip_sst25vf080b_enable_write_status_register();
    microchip_sst25vf080b_write_status_register(second_write);
}

void external_flash_erase_sectors(uint32_t sector_address, uint32_t sector_count) {
    for (size_t sector = 0; sector < sector_count; sector++) {
        const uint32_t sector_size = 4096;
        uint32_t address = sector_address + (sector * sector_size);
        erase_sector(address);
    }
}

void external_flash_read(uint32_t address, uint32_t size, void* out) {
    microchip_sst25vf080b_read(address, size, out);
}

void external_flash_write(uint32_t address, uint32_t size, const void* data) {
    // TODO: This is VERY slow. Replace with the AAI interface.

    const uint8_t* bytes = (const uint8_t*)data;

    for (size_t i = 0; i < size; i++) {
        microchip_sst25vf080b_write_enable();
        microchip_sst25vf080b_byte_program(address + i, bytes[i]);
        wait_until_not_busy();
    }
}
