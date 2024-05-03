MEMORY {
    BOOTLOADER (r) : ORIGIN = BOOTLOADER_ORIGIN, LENGTH = BOOTLOADER_SIZE
    APPLICATION (rx) : ORIGIN = APPLICATION_ORIGIN, LENGTH = APPLICATION_SIZE
    RAM (rw) : ORIGIN = RAM_ORIGIN, LENGTH = RAM_SIZE
}

SECTIONS {
    .text : {
        KEEP(*(.vectors .vectors.*))
        *(.text.*)
        *(.rodata.*)
        _etext = .;
    } > APPLICATION

    .appvec (NOLOAD) : {
        _app_vectors = .;
        KEEP(*(.app_vectors .app_vectors.*))
    } > RAM

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
    } > RAM AT >APPLICATION

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
