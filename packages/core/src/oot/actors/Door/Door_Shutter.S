#include <combo.h>

/* Use our own "door locked" handler */
PATCH_START 0x808b86ec
  move a1,s0
PATCH_END

PATCH_START 0x808b86f4
  jal comboDoorIsUnlocked
   nop
PATCH_END
