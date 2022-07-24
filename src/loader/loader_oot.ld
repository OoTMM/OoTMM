SECTIONS
{
  . = 0x80006600;
  .text : {
    *(.text);
    *(.data);
    *(.rodata);
  }
  /DISCARD/ : { *(.*); }
}
