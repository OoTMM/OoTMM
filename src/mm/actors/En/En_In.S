#include <combo.h>

/* Remove check for garo mask */
PATCH_START 0x808f4c4c
  li t7,0xfff
PATCH_END

PATCH_START 0x808f5310
  li t3,0xfff
PATCH_END
