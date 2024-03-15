#include <combo.h>

/* Hookshot anywhere */
PATCH_GROUP PG_MM_HOOKSHOT_ANYWHERE

PATCH_START 0x808c1840
  nop
PATCH_END

PATCH_GROUP_END

ArmsHook_GetPlayerDisplayList_Hook:
  HOOK_SAVE
  jal   ArmsHook_GetPlayerDisplayList
   nop
  sw    v0, 0x0018 (sp) // store into HOOK_SAVE's T0
  HOOK_RESTORE
  jr    ra
   nop

PATCH_START 0x808c1a5c
  jal   ArmsHook_GetPlayerDisplayList_Hook
   nop
PATCH_END
