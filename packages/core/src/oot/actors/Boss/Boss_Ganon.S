#include <combo.h>

/* Fast ganondorf cutscene */
PATCH_START 0x809f2f3c
  li t0,1
PATCH_END

/* Skip collapse sequence */
PATCH_START 0x8009da40
  li t8,0x517
PATCH_END

PATCH_START 0x809f5320
  li t6,0x517
PATCH_END
