#include <combo.h>

/* Skip intro */
PATCH_START 0x809d1f74
  li t8,1
PATCH_END

PATCH_START 0x809d2040
  li t8,1
PATCH_END

PATCH_START 0x809d2128
  li t7,1
PATCH_END
