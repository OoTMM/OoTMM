OUTPUT_ARCH (mips)
SECTIONS {
    . = 0x80000400 - 0x1000;

    .text :
    {
        *(.text.makerom)
        *(.text.ipl3)
        *(.text.entry)
        *(.text)
        *(.rodata)
        *(.rodata.str*)
        *(.rodata.cst*)
        *(.data)
        *(.bss)
    }

    /DISCARD/ :
    {
        *(*);
    }
}
