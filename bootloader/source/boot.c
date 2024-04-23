#include "boot.h"
#include "syscfg.h"
#include <stdint.h>
#include <string.h>

// Copy the application's vector table from the start of its image into SRAM so it is read afer
// a reset-to-SRAM.
static void copy_application_vectors_to_sram(void) {
    extern uint32_t APPLICATION_ORIGIN;
    extern uint32_t _app_vectors[];

    const size_t vector_table_length = 48;
    const size_t vector_table_size = sizeof(uint32_t) * vector_table_length;

    memcpy(_app_vectors, &APPLICATION_ORIGIN, vector_table_size);
}

noreturn static void jump_to_application_reset_handler(void) {
    extern uint32_t _app_vectors[];

    uint32_t stack_pointer = _app_vectors[0];
    uint32_t reset_handler_vector = _app_vectors[1];
    uint32_t link_register = 0xFFFFFFFF;

    // Unfortunately, a little assembly is required for this.
    // This places `stack_pointer` into the SP register, resets the link register, and then jumps
    // to `reset_handler_vector`.
    __asm__ (
        "mov sp, %0\n"
        "mov lr, %1\n"
        "bx %2\n"
        :
        : "r" (stack_pointer), "r" (link_register), "r" (reset_handler_vector)
    );

    // BX instruction means control never leaves the previous __asm__ block, but the compiler
    // can't tell. This builtin tells the compiler this statement is never reached, and can be
    // optimized out.
    __builtin_unreachable();
}

noreturn void boot_to_application(void) {
    copy_application_vectors_to_sram();
    syscfg_set_mem_mode(syscfg_mem_mode_sram);
    jump_to_application_reset_handler();
}
