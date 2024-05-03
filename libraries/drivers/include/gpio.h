#ifndef DRIVERS_GPIO_H
#define DRIVERS_GPIO_H

#include "driver_addresses.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum gpio {
    gpio_a = GPIOA_BASE_ADDRESS,
    gpio_b = GPIOB_BASE_ADDRESS,
    gpio_c = GPIOC_BASE_ADDRESS,
    gpio_d = GPIOD_BASE_ADDRESS,
    gpio_e = GPIOE_BASE_ADDRESS,
    gpio_f = GPIOF_BASE_ADDRESS,
} gpio_t;

typedef uint8_t gpio_pin_t;

typedef enum gpio_mode {
    gpio_mode_input = 0x0,
    gpio_mode_output = 0x1,
    gpio_mode_alternative = 0x2,
    gpio_mode_analog = 0x3,
} gpio_mode_t;

void gpio_set_mode(gpio_t gpio, gpio_pin_t pin, gpio_mode_t mode);
void gpio_set_alternate_function(gpio_t gpio, gpio_pin_t pin, uint8_t af_number);
void gpio_set_output(gpio_t gpio, gpio_pin_t pin, bool state);

#endif // DRIVERS_GPIO_H
