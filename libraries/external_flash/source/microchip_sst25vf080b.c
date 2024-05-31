#include "microchip_sst25vf080b.h"
#include "rcc.h"
#include "gpio.h"
#include "spi.h"

// SST25VF080B reference manual table 4-4
enum {
    opcode_read = 0x03,
    opcode_high_speed_read = 0x0B,
    opcode_4k_sector_erase = 0x20,
    opcode_32k_block_erase = 0x52,
    opcode_64k_block_erase = 0xD8,
    opcode_chip_erase = 0x60,
    opcode_byte_program = 0x02,
    opcode_aai_word_program = 0xAD,
    opcode_read_status_register = 0x05,
    opcode_enable_write_status_register = 0x50,
    opcode_write_status_register = 0x01,
    opcode_write_enable = 0x06,
    opcode_write_disable = 0x04,
    opcode_read_id = 0x90,
    opcode_jedec_id = 0x9F,
    opcode_enable_busy = 0x70,
    opcode_disable_busy = 0x80,
};

static size_t max(size_t a, size_t b) {
    return (a > b) ? a : b;
}

static void transceive(
    spi_t spi,
    size_t tx_size,
    const uint8_t* tx_data,
    size_t rx_skip,
    size_t rx_size,
    uint8_t* rx_data
) {
    spi_enable(spi);

    // total number of SPI read/writes which need to happen.
    size_t iterations = max(tx_size, rx_skip + rx_size);

    for (size_t i = 0; i < iterations; i++) {
        // Wait until data can be TX'd.
        while (spi_get_tx_fifo_level(spi) == spi_fifo_level_full) {}

        // Transmission
        if (i < tx_size) {
            // Write the next data byte onto the TX FIFO.
            spi_transmit(spi, tx_data[i]);
        } else {
            // Sent all TX data, send an empty byte to keep CLK going.
            spi_transmit(spi, 0x00);
        }

        // Wait until data can be RX'd.
        while (spi_get_rx_fifo_level(spi) == spi_fifo_level_empty) {}

        // Reception
        if ((i < rx_skip) || (i >= (rx_skip + rx_size))) {
            // Still skipping RX'd bytes, read it from the data register and discard it.
            (void)spi_receive(spi);
        } else {
            // After the RX skip, but before the end of the array. Read and store a byte.
            size_t index = i - rx_skip;
            rx_data[index] = spi_receive(spi);
        }
    }

    // Finish transfers
    while (spi_get_tx_fifo_level(spi) != spi_fifo_level_empty) {}
    while (spi_is_busy(spi)) {}

    // Disable SPI (NSS goes idle)
    spi_disable(spi);

    // Finish reception
    while (spi_get_tx_fifo_level(spi) != spi_fifo_level_empty) {
        (void)spi_receive(spi);
    }
}

void microchip_sst25vf080b_initialize(void) {
    // Enable SPI1 peripheral and I/O bank A.
    rcc_apb2_spi1_enable();
    rcc_ahb_iopa_enable();

    // Configure SPI pins' alternative functions to SPI1.
    gpio_set_mode(gpio_a, 4, gpio_mode_alternative);
    gpio_set_mode(gpio_a, 5, gpio_mode_alternative);
    gpio_set_mode(gpio_a, 6, gpio_mode_alternative);
    gpio_set_mode(gpio_a, 7, gpio_mode_alternative);
    gpio_set_alternate_function(gpio_a, 4, 0);
    gpio_set_alternate_function(gpio_a, 5, 0);
    gpio_set_alternate_function(gpio_a, 6, 0);
    gpio_set_alternate_function(gpio_a, 7, 0);

    // Divide the clock speed by `(1 << (1 + 6)) = 128`.
    spi_set_baud_rate_control(spi1, 6);

    // Configure how SPI treats the chip select line.
    spi_set_master_mode(spi1);
    spi_enable_ss_output(spi1);
}

jedec_id_t microchip_sst25vf080b_jedec_id(void) {
    const uint8_t command[] = { opcode_jedec_id };
    uint8_t response[3];
    transceive(spi1, sizeof(command), command, 1, sizeof(response), response);

    return (jedec_id_t){
        .manufacturer = response[0],
        .device_low = response[1],
        .device_high = response[2],
    };
}

void microchip_sst25vf080b_write_enable(void) {
    const uint8_t command[] = { opcode_write_enable };
    transceive(spi1, sizeof(command), command, 0, 0, NULL);
}

uint8_t microchip_sst25vf080b_read_status_register(void) {
    const uint8_t command[] = { opcode_read_status_register };

    uint8_t status;
    transceive(spi1, sizeof(command), command, 1, 1, &status);

    return status;
}

void microchip_sst25vf080b_enable_write_status_register(void) {
    const uint8_t command[] = { opcode_enable_write_status_register };
    transceive(spi1, sizeof(command), command, 0, 0, NULL);
}

void microchip_sst25vf080b_write_status_register(uint8_t value) {
    const uint8_t command[] = { opcode_write_status_register, value };
    transceive(spi1, sizeof(command), command, 0, 0, NULL);
}

void microchip_sst25vf080b_erase_4k_sector(uint32_t address) {
    uint8_t addr_0 = (uint8_t)(address >> 16);
    uint8_t addr_1 = (uint8_t)(address >> 8);
    uint8_t addr_2 = (uint8_t)address;
    uint8_t command[] = { opcode_4k_sector_erase, addr_0, addr_1, addr_2 };

    transceive(spi1, sizeof(command), command, 0, 0, NULL);
}

void microchip_sst25vf080b_byte_program(uint32_t address, uint8_t data) {
    uint8_t addr_0 = (uint8_t)(address >> 16);
    uint8_t addr_1 = (uint8_t)(address >> 8);
    uint8_t addr_2 = (uint8_t)address;
    uint8_t command[] = { opcode_byte_program, addr_0, addr_1, addr_2, data };

    transceive(spi1, sizeof(command), command, 0, 0, NULL);
}

void microchip_sst25vf080b_read(uint32_t address, size_t size, uint8_t* data) {
    uint8_t addr_0 = (uint8_t)(address >> 16);
    uint8_t addr_1 = (uint8_t)(address >> 8);
    uint8_t addr_2 = (uint8_t)address;
    uint8_t command[] = { opcode_read, addr_0, addr_1, addr_2 };

    transceive(spi1, sizeof(command), command, sizeof(command), size, data);
}
