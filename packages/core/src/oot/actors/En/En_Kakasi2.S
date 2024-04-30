#include <combo.h>

/* No cutscne when Pierre spawns */
PATCH_START 0x80b7990c
  nop
PATCH_END

PATCH_GROUP PG_OOT_FREE_SCARECROW

FreeScarecrow:
  lhu  t0, 0x670 (v0)         /* Load Link's StateII */
  andi at, t0, 0x0800         /* Mask it with playing_ocarina bit */
  li   t0, 0x0800
  jr   ra
  nop

PATCH_START 0x80b79878
  jal FreeScarecrow
    nop
PATCH_END

PATCH_GROUP_END
