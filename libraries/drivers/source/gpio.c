#include "gpio.h"
#include "critical_section.h"

static volatile uint32_t* get_register(gpio_t gpio, uint32_t register_offset) {
    return (volatile uint32_t*)((uintptr_t)gpio + register_offset);
}

void gpio_set_mode(gpio_t gpio, gpio_pin_t pin, gpio_mode_t mode) {
    // Pin modes are packed into 2-bit values in the gpio bank's MODER register.
    uint32_t offset = pin * 2;
    uint32_t mask = 0x3 << offset;
    uint32_t bits = ((uint32_t)mode << offset) & mask;

    volatile uint32_t* moder = get_register(gpio, GPIOx_MODER_OFFSET);

    CRITICAL_SECTION_ENTER();
    uint32_t value = *moder;
    value &= ~mask;
    value |= bits;
    *moder = value;
    CRITICAL_SECTION_EXIT();
}

void gpio_set_alternate_function(gpio_t gpio, gpio_pin_t pin, uint8_t af_number) {
    // Alternate function config is split over two banks, the low 8 pins are in AFRL, the high 8
    // pins in AFRH.
    bool pin_is_in_lower_bank = (pin < 8);

    // Calculate the offset into the register the pin belongs to.
    uint32_t offset_in_register = pin_is_in_lower_bank ? (uint32_t)(pin * 4) : (uint32_t)(4 * (pin - 8));

    // Retrieve the register which needs to be updated.
    uintptr_t register_offset = pin_is_in_lower_bank ? GPIOx_AFRL_OFFSET : GPIOx_AFRH_OFFSET;
    volatile uint32_t* af_bank = get_register(gpio, register_offset);

    // Pin AF number values are packed into 4-bit values in the register.
    uint32_t mask = 0x7 << offset_in_register;
    uint32_t bits = (af_number << offset_in_register) & mask;

    CRITICAL_SECTION_ENTER();
    uint32_t value = *af_bank;
    value &= ~mask;
    value |= bits;
    *af_bank = value;
    CRITICAL_SECTION_EXIT();
}

void gpio_set_output(gpio_t gpio, gpio_pin_t pin, bool state) {
    // Low half-word of BSRR _sets_ a pin, high half-word _resets_ a pin.
    uint8_t offset = state ? 0 : 16;

    // Only want to consider least significant 4 bits.
    pin &= 0xF;

    // Determine which bit to write to.
    uint32_t bit = 1 << (pin + offset);

    // Atomic write, no critical section required.
    volatile uint32_t* bsrr = get_register(gpio, GPIOx_BSRR_OFFSET);
    *bsrr = bit;
}
