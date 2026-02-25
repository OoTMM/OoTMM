#include <combo.h>

/* Fast Block Pushing */
PATCH_START 0x80a56c40
  lui at,0x4080
PATCH_END

PATCH_START 0x80a56ddc
  li t3,1
PATCH_END
