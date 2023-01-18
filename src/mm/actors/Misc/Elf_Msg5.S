#include <combo.h>

/* Make Tatl shut up */
PATCH_START 0x80afdb48
  jr ra
   nop
PATCH_END
