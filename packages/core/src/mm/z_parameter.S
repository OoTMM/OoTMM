#include <combo.h>

Interface_GetCustomIconTexture_Hook:
  HOOK_SAVE
  jal   Interface_GetCustomIconTexture
   lw   a0, 0x98+0x50 (sp)
  sw    v0, 0x0024 (sp) // store into HOOK_SAVE's T3
  HOOK_RESTORE
  jr    ra
   nop

PATCH_START 0x80119310
  jal     Interface_GetCustomIconTexture_Hook
PATCH_END

Interface_GetItemRestriction_Hook:
  HOOK_SAVE
  or    a1, a2, zero
  jal   Interface_GetItemRestriction
   or   a2, s0, zero
  sw    v0, 0x003c (sp) // store into HOOK_SAVE's T9
  HOOK_RESTORE
  jr    ra
   nop

PATCH_START 0x80110fd0
  jal     Interface_GetItemRestriction_Hook
   nop
PATCH_END
