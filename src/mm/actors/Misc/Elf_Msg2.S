#include <combo.h>

/* Make Tatl shut up */
PATCH_START 0x8096f04c
  jr ra
   nop
PATCH_END
