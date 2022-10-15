#include <combo.h>

/* Disable the message check for zora tunic */
PATCH_START 0x80ad6454
  li t3,0x4012
PATCH_END

PATCH_START 0x80ad5e24
  li v0,0x4012
PATCH_END

/* Disable the trade quest timer */
PATCH_START 0x80ad6d1c
  li t9,0xff
PATCH_END
