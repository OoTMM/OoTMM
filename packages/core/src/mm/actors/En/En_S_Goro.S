#include <combo.h>

/* GI renumbering */
PATCH_START 0x80bbee58
  li a2,GI_MM_POWDER_KEG
PATCH_END

PATCH_START 0x80bbef14
  li a2,GI_MM_POWDER_KEG
PATCH_END

EnSGoro_OverrideBombshopGoronTextId_Hook:
  j         EnSGoro_OverrideBombshopGoronTextId
   move     a0, a2 /* This isn't necessarily always current actor, but it is for the use-cases. */

/*
Replaces:
  jr         ra
   nop
*/
PATCH_START 0x80bbd8e8
  j         EnSGoro_OverrideBombshopGoronTextId_Hook
   move     a1, v0
PATCH_END
