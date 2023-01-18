#include <combo.h>

/* Make Tatl shut up */
PATCH_START 0x80afd7dc
  jr ra
   nop
PATCH_END
