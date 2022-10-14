#include <combo.h>

/* Disable Sheik near the Master Sword */
PATCH_START 0x808e2d6c
  li t6,1
PATCH_END
