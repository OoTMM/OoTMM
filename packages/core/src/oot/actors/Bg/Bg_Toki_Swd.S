#include <combo.h>

/* Hide the Master Sword */
PATCH_START 0x80918f7c
  nop
  nop
PATCH_END
