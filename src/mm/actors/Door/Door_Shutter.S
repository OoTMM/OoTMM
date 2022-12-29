#include <combo.h>

/* Remove secret shrine requirements */
PATCH_START 0x808a11b0
  li t6,0x280
PATCH_END
