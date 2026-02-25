#include <combo.h>

/* Make Tatl shut up */
PATCH_START 0x8092e38c
  jr ra
   nop
PATCH_END
