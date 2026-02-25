#include <combo.h>

/* Disable Sheik near the Master Sword */
PATCH_START 0x808e2d6c
  li t6,1
PATCH_END

/* Change Sheik Object ID */
PATCH_START 0x808e3684
.short 0x0001
PATCH_END
