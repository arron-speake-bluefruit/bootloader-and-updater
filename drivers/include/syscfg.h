#ifndef DRIVERS_SYSCFG_H
#define DRIVERS_SYSCFG_H

typedef enum syscfg_mem_mode {
    syscfg_mem_mode_main_flash = 0x0,
    syscfg_mem_mode_system_flash = 0x1,
    syscfg_mem_mode_sram = 0x3,
} syscfg_mem_mode_t;

// Control the memory internal mapping at address `0x00000000`. After reset these bits take on the
// value selected by the actual boot mode configuration.
void syscfg_set_mem_mode(syscfg_mem_mode_t mode);

#endif // DRIVERS_SYSCFG_H
