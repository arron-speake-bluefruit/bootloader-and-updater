#ifndef DRIVERS_GPIO_H
#define DRIVERS_GPIO_H

#include <stdint.h>
#include <stdbool.h>

typedef enum gpio {
    gpio_a = (uintptr_t)0x48000000,
    gpio_b = (uintptr_t)0x48000400,
    gpio_c = (uintptr_t)0x48000800,
    gpio_d = (uintptr_t)0x48000C00,
    gpio_e = (uintptr_t)0x48001000,
    gpio_f = (uintptr_t)0x48001400,
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
