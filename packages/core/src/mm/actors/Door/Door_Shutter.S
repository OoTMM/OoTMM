#include <combo.h>

/* Remove secret shrine requirements */
PATCH_START 0x808a11b0
  li t6,0x280
PATCH_END

/* Use our own "locked door" hook */
PATCH_START 0x808a0c94
  move a1,s0
PATCH_END

PATCH_START 0x808a0c9c
  jal comboDoorIsUnlocked
   nop
PATCH_END
