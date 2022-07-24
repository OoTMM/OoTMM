SECTIONS
{
  . = 0x80400000;
  .text : {
    *(.text);
    *(.data);
    *(.rodata);
  }
  /DISCARD/ : { *(.*); }
}
