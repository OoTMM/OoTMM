#include <combo.h>

/* Always use the extended map for graveyard */
PATCH_START 0x8006bf94
  li t2,1
PATCH_END

/* Always use the extended map for gerudo fortress */
PATCH_START 0x8006c054
  move t2,at
PATCH_END

/* Use the extended lake map when the water isn't raised */
IsWaterRaised:
  la  t6,gSaveContext
  lhu t6,(0xed4+((EV_OOT_CHK_LAKE_HYLIA_WATER >> 4) * 2))(t6)
  li  t5,(1 << (EV_OOT_CHK_LAKE_HYLIA_WATER & 0xf))
  jr ra
   and t6,t5

PATCH_START 0x8006bfd8
  jal IsWaterRaised
   nop
  nop
PATCH_END
