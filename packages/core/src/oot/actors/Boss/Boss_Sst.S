#include <combo.h>

/* Skip boss intro */
PATCH_START 0x80a18120
  li t0,1
PATCH_END

PATCH_START 0x80a1841c
  li t4,1
PATCH_END

PATCH_START 0x80a18c30
  li v1,1
PATCH_END
