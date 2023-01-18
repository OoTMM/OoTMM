#include <combo.h>

/* Make Tatl shut up */
PATCH_START 0x8096ec4c
  jr ra
   nop
PATCH_END
