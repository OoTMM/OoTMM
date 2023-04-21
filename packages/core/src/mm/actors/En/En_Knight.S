#include <combo.h>

/* Remove the death cutscene */
PATCH_START 0x809b5854
  li t8,0
PATCH_END

/* Remove the intro cutscene */
PATCH_START 0x809b2c74
  lw t0,0x331c(t0)
PATCH_END

PATCH_START 0x809b2c80
  nop
  bnez t0, . + 0x110
PATCH_END
