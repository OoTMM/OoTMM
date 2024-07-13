SECTIONS
{
  . = 0x80d00000;

  .text :   ALIGN(16) { SORT(*)(.text*); }
  .data :   ALIGN(16) { SORT(*)(.data*); SORT(*)(.bss*); }
  .rodata : ALIGN(16) { SORT(*)(.rodata*); }
  .bss :    ALIGN(16) { }

  .meta : { SORT(*)(.meta*); }
  /DISCARD/ : { *(.*); }
}
