#include <combo.h>

/* Hookshot anywhere */
PATCH_GROUP PG_OOT_HOOKSHOT_ANYWHERE

PATCH_START 0x80910378
  nop
PATCH_END

PATCH_GROUP_END

/* Don't require the adult object */
PATCH_START 0x809106e8
.short 0x0001
PATCH_END

DrawHook1:
  /* Replaced code */
  lwc1 $f12,0x24(s0)
  lwc1 $f14,0x28(s0)

  lw   t0,(gSaveContext+0x04)
  beqz t0,DrawHook1_End
   nop

  /* Disable display list */
  sw zero,0(v1)
  sw zero,4(v1)

DrawHook1_End:
  jr ra
   nop

/* Disable hookshot display as child */
PATCH_START 0x809105c8
  jal DrawHook1
   nop
PATCH_END

DrawHook2:
  lw   t0,(gSaveContext+0x04)
  beqz t0,DrawHook2_End
   nop

  /* Disable display list */
  move t4,zero
  move t5,zero

DrawHook2_End:
  sw t4,0x0(v1)
  sw t5,0x4(v1)
  jr ra
   nop

PATCH_START 0x809106b8
  jal DrawHook2
   nop
PATCH_END
