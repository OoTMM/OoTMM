SECTIONS
{
  .text : { SORT(*)(.text*); }
  .data : { SORT(*)(.data*); }
  .rodata : { SORT(*)(.rodata*); }
  .bss : { SORT(*)(.bss*); }
  .meta : { SORT(*)(.meta*); }
}
