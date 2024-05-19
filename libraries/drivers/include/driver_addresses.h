#ifndef DRIVERS_DRIVER_ADDRESSES_H
#define DRIVERS_DRIVER_ADDRESSES_H

// Internal drivers header containing constants for the locations of peripherals & headers.
// Not all peripherals defined here are available on the STM32F05xx series.
// Peripheral register offsets will be added as needed.

#include <stdint.h>

// STM32F0xx Core Peripheral Addresses, STM32F0xx Cortex-M0 Programming Manual Table 24

#define SCB_BASE_ADDRESS ((uintptr_t)0xE000E008)
#define STK_BASE_ADDRESS ((uintptr_t)0xE000E010)
#define NVIC_BASE_ADDRESS ((uintptr_t)0xE000E100)
#define SCB_BASE_ADDRESS_2 ((uintptr_t)0xE000ED00)
#define NVIC_BASE_ADDRESS_2 ((uintptr_t)0xE000EF00)

// STM32F0xx Peripheral Addresses, STM32F0xx Reference Manual Table 1

#define GPIOF_BASE_ADDRESS ((uintptr_t)0x48001400)
#define GPIOE_BASE_ADDRESS ((uintptr_t)0x48001000)
#define GPIOD_BASE_ADDRESS ((uintptr_t)0x48000C00)
#define GPIOC_BASE_ADDRESS ((uintptr_t)0x48000800)
#define GPIOB_BASE_ADDRESS ((uintptr_t)0x48000400)
#define GPIOA_BASE_ADDRESS ((uintptr_t)0x48000000)
#define TSC_BASE_ADDRESS ((uintptr_t)0x40024000)
#define CRC_BASE_ADDRESS ((uintptr_t)0x40023000)
#define FLASH_BASE_ADDRESS ((uintptr_t)0x40022000)
#define RCC_BASE_ADDRESS ((uintptr_t)0x40021000)
#define DMA2_BASE_ADDRESS ((uintptr_t)0x40020400)
#define DMA_BASE_ADDRESS ((uintptr_t)0x40020000)
#define DBGMCU_BASE_ADDRESS ((uintptr_t)0x40015800)
#define TIM17_BASE_ADDRESS ((uintptr_t)0x40014800)
#define TIM16_BASE_ADDRESS ((uintptr_t)0x40014400)
#define TIM15_BASE_ADDRESS ((uintptr_t)0x40014000)
#define USART1_BASE_ADDRESS ((uintptr_t)0x40013800)
#define SPI1_BASE_ADDRESS ((uintptr_t)0x40013000)
#define I2S1_BASE_ADDRESS ((uintptr_t)0x40013000)
#define TIM1_BASE_ADDRESS ((uintptr_t)0x40012C00)
#define ADC_BASE_ADDRESS ((uintptr_t)0x40012400)
#define USART8_BASE_ADDRESS ((uintptr_t)0x40011C00)
#define USART7_BASE_ADDRESS ((uintptr_t)0x40011800)
#define USART6_BASE_ADDRESS ((uintptr_t)0x40011400)
#define EXTI_BASE_ADDRESS ((uintptr_t)0x40010400)
#define SYSCFG_BASE_ADDRESS ((uintptr_t)0x40010000)
#define COMP_BASE_ADDRESS ((uintptr_t)0x40010000)
#define CEC_BASE_ADDRESS ((uintptr_t)0x40007800)
#define DAC_BASE_ADDRESS ((uintptr_t)0x40007400)
#define PWR_BASE_ADDRESS ((uintptr_t)0x40007000)
#define CRS_BASE_ADDRESS ((uintptr_t)0x40006C00)
#define CAN_BASE_ADDRESS ((uintptr_t)0x40006400)
#define USB_CAN_SRAM_BASE_ADDRESS ((uintptr_t)0x40006000)
#define USB_BASE_ADDRESS ((uintptr_t)0x40005C00)
#define I2C2_BASE_ADDRESS ((uintptr_t)0x40005800)
#define I2C1_BASE_ADDRESS ((uintptr_t)0x40005400)
#define USART5_BASE_ADDRESS ((uintptr_t)0x40005000)
#define USART4_BASE_ADDRESS ((uintptr_t)0x40004C00)
#define USART3_BASE_ADDRESS ((uintptr_t)0x40004800)
#define USART2_BASE_ADDRESS ((uintptr_t)0x40004400)
#define SPI2_BASE_ADDRESS ((uintptr_t)0x40003800)
#define IWDG_BASE_ADDRESS ((uintptr_t)0x40003000)
#define WWDG_BASE_ADDRESS ((uintptr_t)0x40002C00)
#define RTC_BASE_ADDRESS ((uintptr_t)0x40002800)
#define TIM14_BASE_ADDRESS ((uintptr_t)0x40002000)
#define TIM7_BASE_ADDRESS ((uintptr_t)0x40001400)
#define TIM6_BASE_ADDRESS ((uintptr_t)0x40001000)
#define TIM3_BASE_ADDRESS ((uintptr_t)0x40000400)
#define TIM2_BASE_ADDRESS ((uintptr_t)0x40000000)

// FLASH peripheral registers, STM32F0xx Reference Manual Table 9

#define FLASH_ACR_OFFSET ((uintptr_t)0x00)
#define FLASH_KEYR_OFFSET ((uintptr_t)0x04)
#define FLASH_OPTKEYR_OFFSET ((uintptr_t)0x08)
#define FLASH_SR_OFFSET ((uintptr_t)0x0C)
#define FLASH_CR_OFFSET ((uintptr_t)0x10)
#define FLASH_AR_OFFSET ((uintptr_t)0x14)
#define FLASH_OBR_OFFSET ((uintptr_t)0x1C)
#define FLASH_WRPR_OFFSET ((uintptr_t)0x20)

// GPIO peripheral registers, STM32F0xx Reference Manual Table 25

#define GPIOx_MODER_OFFSET ((uintptr_t)0x00)
#define GPIOx_OTYPER_OFFSET ((uintptr_t)0x04)
#define GPIOx_OSPEEDR_OFFSET ((uintptr_t)0x08)
#define GPIOx_PUPDR_OFFSET ((uintptr_t)0x0C)
#define GPIOx_IDR_OFFSET ((uintptr_t)0x10)
#define GPIOx_ODR_OFFSET ((uintptr_t)0x14)
#define GPIOx_BSRR_OFFSET ((uintptr_t)0x18)
#define GPIOx_LCKR_OFFSET ((uintptr_t)0x1C)
#define GPIOx_AFRL_OFFSET ((uintptr_t)0x20)
#define GPIOx_AFRH_OFFSET ((uintptr_t)0x24)
#define GPIOx_BRR_OFFSET ((uintptr_t)0x28)

// NVIC peripheral registers, STM32F0xx Cortex-M0 Programming Manual Table 25

#define NVIC_ISER_ADDRESS ((uintptr_t)0xE000E100)
#define NVIC_ICER_ADDRESS ((uintptr_t)0XE000E180)
#define NVIC_ISPR_ADDRESS ((uintptr_t)0XE000E200)
#define NVIC_ICPR_ADDRESS ((uintptr_t)0XE000E280)
#define NVIC_IPR0_ADDRESS ((uintptr_t)0xE000E400)

// RCC peripheral registers, STM32F0xx Reference Manual Table 19

#define RCC_CR_OFFSET ((uintptr_t)0x00)
#define RCC_CFGR_OFFSET ((uintptr_t)0x04)
#define RCC_CIR_OFFSET ((uintptr_t)0x08)
#define RCC_APB2RSTR_OFFSET ((uintptr_t)0x0C)
#define RCC_APB1RSTR_OFFSET ((uintptr_t)0x10)
#define RCC_AHBENR_OFFSET ((uintptr_t)0x14)
#define RCC_APB2ENR_OFFSET ((uintptr_t)0x18)
#define RCC_APB1ENR_OFFSET ((uintptr_t)0x1C)
#define RCC_BDCR_OFFSET ((uintptr_t)0x20)
#define RCC_CSR_OFFSET ((uintptr_t)0x24)
#define RCC_AHBRSTR_OFFSET ((uintptr_t)0x28)
#define RCC_CFGR2_OFFSET ((uintptr_t)0x2C)
#define RCC_CFGR3_OFFSET ((uintptr_t)0x30)
#define RCC_CR2_OFFSET ((uintptr_t)0x34)

// SCB peripheral registers, STM32F0xx Cortex-M0 Programming Manual Table 30

#define SCB_CPUID_ADDRESS ((uintptr_t)0xE000ED00)
#define SCB_ICSR_ADDRESS ((uintptr_t)0xE000ED04)
#define SCB_AIRCR_ADDRESS ((uintptr_t)0xE000ED0C)
#define SCB_SCR_ADDRESS ((uintptr_t)0xE000ED10)
#define SCB_CCR_ADDRESS ((uintptr_t)0xE000ED14)
#define SCB_SHPR2_ADDRESS ((uintptr_t)0xE000ED1C)
#define SCB_SHPR3_ADDRESS ((uintptr_t)0xE000ED20)

// SYSCFG peripheral registers, STM32F0xx Reference Manual Table 26

#define SYSCFG_CFGR1_OFFSET ((uintptr_t)0x00)
#define SYSCFG_EXTICR1_OFFSET ((uintptr_t)0x08)
#define SYSCFG_EXTICR2_OFFSET ((uintptr_t)0x0C)
#define SYSCFG_EXTICR3_OFFSET ((uintptr_t)0x10)
#define SYSCFG_EXTICR4_OFFSET ((uintptr_t)0x14)
#define SYSCFG_CFGR2_OFFSET ((uintptr_t)0x18)

// TIM6/TIM7 peripheral registers, STM32F0xx Reference Manual Table 76

#define TIMx_CR1_OFFSET ((uintptr_t)0x00)
#define TIMx_CR2_OFFSET ((uintptr_t)0x04)
#define TIMx_DIER_OFFSET ((uintptr_t)0x0C)
#define TIMx_SR_OFFSET ((uintptr_t)0x10)
#define TIMx_EGR_OFFSET ((uintptr_t)0x14)
#define TIMx_CNT_OFFSET ((uintptr_t)0x24)
#define TIMx_PSC_OFFSET ((uintptr_t)0x28)
#define TIMx_ARR_OFFSET ((uintptr_t)0x2C)

// USART peripheral registers, STM32F0xx Reference Manual Table 112

#define USART_CR1_OFFSET ((uintptr_t)0x00)
#define USART_CR2_OFFSET ((uintptr_t)0x04)
#define USART_CR3_OFFSET ((uintptr_t)0x08)
#define USART_BRR_OFFSET ((uintptr_t)0x0C)
#define USART_GTPR_OFFSET ((uintptr_t)0x10)
#define USART_RTOR_OFFSET ((uintptr_t)0x14)
#define USART_RQR_OFFSET ((uintptr_t)0x18)
#define USART_ISR_OFFSET ((uintptr_t)0x1C)
#define USART_ICR_OFFSET ((uintptr_t)0x20)
#define USART_RDR_OFFSET ((uintptr_t)0x24)
#define USART_TDR_OFFSET ((uintptr_t)0x28)

// SPI peripheral registers, STM32F0xx Reference Manual Table 117

#define SPIx_CR1_OFFSET ((uintptr_t)0x00)
#define SPIx_CR2_OFFSET ((uintptr_t)0x04)
#define SPIx_SR_OFFSET ((uintptr_t)0x08)
#define SPIx_DR_OFFSET ((uintptr_t)0x0C)
#define SPIx_CRCPR_OFFSET ((uintptr_t)0x10)
#define SPIx_RXCRCR_OFFSET ((uintptr_t)0x14)
#define SPIx_TXCRCR_OFFSET ((uintptr_t)0x18)
#define SPIx_I2SCFGR_OFFSET ((uintptr_t)0x1C)
#define SPIx_I2SPR_OFFSET ((uintptr_t)0x20)

#endif // DRIVERS_DRIVER_ADDRESSES_H
