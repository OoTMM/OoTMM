SECTIONS
{
  . = 0x80800000;
  .text : { SORT(*)(.text*); }
  .data : { SORT(*)(.data*); }
  .rodata : { SORT(*)(.rodata*); }
  .bss : { SORT(*)(.bss*); }
  /DISCARD/ : { *(.*); }
}
