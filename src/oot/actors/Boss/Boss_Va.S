#include <combo.h>

/* Don't spawn Ruto in blue warp */
PATCH_START 0x80985ac4
  nop
  nop
PATCH_END

PATCH_START 0x809896d8
  li a2,0x5d
PATCH_END

/* Fast boss intro */
PATCH_START 0x80985b74
  li t8,1
PATCH_END
