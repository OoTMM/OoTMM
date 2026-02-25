#include <combo.h>

/* Disable timer */
PATCH_START 0x80aac7a8
  nop
PATCH_END

PATCH_START 0x80aac7c4
  nop
PATCH_END

/* Disable check for Eye drops */
PATCH_START 0x80aacddc
  li t9,0xfff
PATCH_END
