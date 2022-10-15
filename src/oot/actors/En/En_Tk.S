#include <combo.h>

/* Add a collectible flag the the dampe HP */
PATCH_START 0x80926dcc
.int 0x1f06
PATCH_END

/* Hook after dampe init */
PATCH_START 0x80926488
  j EnTk_AfterInit
PATCH_END

/* Skip the dampe intro */
PATCH_START 0x80925e2c
  nop
PATCH_END
