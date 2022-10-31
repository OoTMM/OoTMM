#include <combo.h>

/* Remove the lullaby check */
PATCH_START 0x80b746d0
  li t6,0
PATCH_END

PATCH_START 0x80b746e4
  li t8,0
PATCH_END

/* Remove the goron baby event check */
PATCH_START 0x80b74684
  li t9,0xff
PATCH_END
