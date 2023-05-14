#include <combo.h>

/* Use our own "door locked" handler */
PATCH_START 0x80867c1c
  move a1,a2
PATCH_END

PATCH_START 0x80867c2c
  jal comboDoorIsUnlocked
   nop
PATCH_END
