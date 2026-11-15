#include <combo.h>

#if defined(GAME_MM)
PATCH_START 0x800c1dec
  j BgCheck_LoadPolyEpoch
   sw t9,0x54(sp)
PATCH_END

BgCheck_LoadPolyEpoch:
  lui s5,%hi(gBgCheckPolyEpoch)
  j 0x800c1df4
   lbu s5,%lo(gBgCheckPolyEpoch)(s5)
PATCH_START 0x800c1e4c
  .int 0x52b90021
PATCH_END
#else
PATCH_START 0x8002d204
  j BgCheck_LoadPolyEpoch
   lhu t0,0xa2(sp)
PATCH_END

BgCheck_LoadPolyEpoch:
  lui at,%hi(gBgCheckPolyEpoch)
  j 0x8002d20c
   lbu at,%lo(gBgCheckPolyEpoch)(at)
PATCH_START 0x8002d24c
  move t8,at
PATCH_END

PATCH_START 0x8002d264
  move t8,at
PATCH_END
#endif
