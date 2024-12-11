OUTPUT_ARCH (mips)
SECTIONS {
    .text :
    {
        *(.makerom)
        *(.text)
    }

    .data :
    {
        *(.data)
        *(.bss)
    }

    .rodata :
    {
        *(.rodata)
        *(.rodata.str*)
        *(.rodata.cst*)
    }
}
