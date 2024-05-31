#ifndef DRIVERS_SPI_H
#define DRIVERS_SPI_H

#include "driver_addresses.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum spi {
    spi1 = SPI1_BASE_ADDRESS,
    spi2 = SPI2_BASE_ADDRESS,
} spi_t;

typedef enum spi_fifo {
    spi_fifo_16bit = 0,
    spi_fifo_8bit = 1,
} spi_fifo_t;

typedef enum spi_fifo_level {
    spi_fifo_level_empty = 0x0,
    spi_fifo_level_quarter = 0x1,
    spi_fifo_level_half = 0x2,
    spi_fifo_level_full = 0x3,
} spi_fifo_level_t;

// Enable SPI peripheral.
void spi_enable(spi_t spi);

// Disable SPI peripheral.
void spi_disable(spi_t spi);

// Push a byte onto `spi`'s transmission FIFO.
void spi_transmit(spi_t spi, uint8_t data);

// Pull a byte from `spi`'s reception FIFO.
uint8_t spi_receive(spi_t spi);

// Report the fullness of `spi`'s TX FIFO.
spi_fifo_level_t spi_get_tx_fifo_level(spi_t spi);

// Report the fullness of `spi`'s RX FIFO.
spi_fifo_level_t spi_get_rx_fifo_level(spi_t spi);

// Report whether `spi`'s busy bit is set.
bool spi_is_busy(spi_t spi);

// Set the clock baud rate for `spi` to `F_PCLK / (1 << (n + 1))`. `n` cannot exceed 7.
void spi_set_baud_rate_control(spi_t spi, uint8_t n);

// Configure `spi` to operate as a SPI master.
void spi_set_master_mode(spi_t spi);

// Enable SS for `spi` while in master mode and enabled.
void spi_enable_ss_output(spi_t spi);

#endif // DRIVERS_SPI_H
