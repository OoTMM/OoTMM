SECTIONS
{
  .text : { SORT(*)(.text*); }
  .data : { SORT(*)(.data*); SORT(*)(.bss*); }
  .rodata : { SORT(*)(.rodata*); }
  .meta : { SORT(*)(.meta*); }

  .symbols_addr : { SORT(*)(.symbols_addr); }
  .symbols_name : { SORT(*)(.symbols_name); }

  /DISCARD/ : { *(.*); }
}
