#include <combo.h>

/* Fast block pushing */
PATCH_START 0x80a50144
  lui at,0x4080
PATCH_END

PATCH_START 0x80a502e4
  li t2,1
PATCH_END
