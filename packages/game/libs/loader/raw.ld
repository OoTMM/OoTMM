OUTPUT_ARCH (mips)
SECTIONS {
    .text :
    {
        *(.text)
    }
    
    /DISCARD/ :
    {
        *(*);
    }
}
