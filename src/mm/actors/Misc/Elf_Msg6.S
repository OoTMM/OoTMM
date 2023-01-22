#include <combo.h>

/* Make Tatl shut up */
PATCH_START 0x80ba22b0
  jr ra
   nop
PATCH_END
