OUTPUT_ARCH (mips)
SECTIONS {
    .text :
    {
        *(.text.makerom)
        *(.text.ipl3)
        *(.text.entry)
        *(.text)
        *(.data)
        *(.rodata)
        *(.rodata.str*)
        *(.rodata.cst*)
    }

    .bss :
    {
        *(.bss)
    }

    /DISCARD/ :
    {
        *(*);
    }
}
