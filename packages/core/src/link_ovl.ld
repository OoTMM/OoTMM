SECTIONS
{
  .text : { SORT(*)(.text*); }
  .data : { SORT(*)(.data*); SORT(*)(.bss*); }
  .rodata : { SORT(*)(.rodata*); }
  .meta : { SORT(*)(.meta*); }

  /DISCARD/ : { *(.*); }
}
