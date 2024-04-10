#ifndef DRIVERS_RCC_H
#define DRIVERS_RCC_H

// Enable the I/O port A clock.
void rcc_ahb_iopa_enable(void);

// Enable the I/O port C clock.
void rcc_ahb_iopc_enable(void);

// Enable the USART1 clock.
void rcc_apb2_usart1_enable(void);

// Enable the USART2 clock.
void rcc_apb1_usart2_enable(void);

// Disable the I/O port A clock.
void rcc_ahb_iopa_disable(void);

// Disable the USART2 clock.
void rcc_apb1_usart2_disable(void);

#endif // DRIVERS_RCC_H
