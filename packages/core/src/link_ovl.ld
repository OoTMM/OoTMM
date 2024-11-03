SECTIONS
{
  .text : { SORT(*)(.text*); }
  .data : { SORT(*)(.data*); SORT(*)(.bss*); }
  .rodata : { SORT(*)(.rodata*); }
  .meta : { SORT(*)(.meta*); }

  .cosmetic_addr : { SORT(*)(.cosmetic_addr); }
  .cosmetic_name : { SORT(*)(.cosmetic_name); }

  /DISCARD/ : { *(.*); }
}
