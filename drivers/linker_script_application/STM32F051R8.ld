STACK_SIZE = 0x1000;

MEMORY {
    BOOTLOADER (r) : ORIGIN = 0x08000000, LENGTH = 4k
    FLASH (rx) : ORIGIN = 0x08001000, LENGTH = 60k
    RAM (rw) : ORIGIN =  0x20000000, LENGTH = 8k
}

ENTRY(reset_handler);

SECTIONS {
    .text : {
        *(.text.*)
        *(.rodata.*)
        _etext = .;
    } > FLASH

    .bss (NOLOAD) : {
        _app_vectors = .;
        KEEP(*(.app_vectors .app_vectors.*))
        _sbss = . ;
        *(.bss .bss.*)
        *(COMMON)
        _ebss = . ;
    } > RAM

    .data : {
        _sdata = .;
        *(.data*);
        _edata = .;
    } > RAM AT >FLASH

    .stack (NOLOAD): {
        . = ALIGN(8);
        _sstack = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _estack = .;
    } > RAM

    . = ALIGN(4);
    _end = . ;
}
