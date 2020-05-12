OUTPUT_FORMAT(binary)
SECTIONS
{
  . = 0x807f0000;

  .text ALIGN(0x4) :    { *(.text) *(.data) *(.rodata) }
  /DISCARD/ : { *(.*) }
}
