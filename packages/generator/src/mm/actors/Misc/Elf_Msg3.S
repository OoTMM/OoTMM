#include <combo.h>

/* Make Tatl shut up */
PATCH_START 0x80a2d0fc
  jr ra
   nop
PATCH_END
