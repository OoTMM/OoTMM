#include <combo.h>

/* Disable tunic stealing */
PATCH_START 0x809e7260
  li t1,1
PATCH_END
