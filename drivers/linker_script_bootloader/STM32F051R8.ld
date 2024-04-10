STACK_SIZE = 0x1000;

_application_start = 0x08001000;

MEMORY {
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 4k
    APPLICATION (r) : ORIGIN = _application_start, LENGTH = 60k
    RAM (rw) : ORIGIN =  0x20000000, LENGTH = 8k
}

ENTRY(reset_handler);

SECTIONS {
    .text : {
        KEEP(*(.vectors .vectors.*))
        *(.text.*)
        *(.rodata.*)
        _etext = .;
    } > FLASH

    .bss (NOLOAD) : {
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
