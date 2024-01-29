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

/*
;==================================================================================================
; Check if items usability should be checked while swimming
;==================================================================================================
*/

Items_ShouldCheckItemUsabilityWhileSwimming_Hook:
    addiu   sp, sp, -0x20
    sw      ra, 0x001C (sp)
    sw      a1, 0x0024 (sp)
    sw      a3, 0x002C (sp)
    sw      t3, 0x0018 (sp)
    sw      t4, 0x0014 (sp)

    jal     Items_ShouldCheckItemUsabilityWhileSwimming
     or     a1, a2, zero

    lw      t4, 0x0014 (sp)
    lw      t3, 0x0018 (sp)
    lw      a3, 0x002C (sp)
    lw      a1, 0x0024 (sp)
    lw      ra, 0x001C (sp)
    jr      ra
     addiu  sp, sp, 0x20

/*
; Replaces:
;   ANDI    T7, A2, 0x00FF
;   ADDIU   AT, R0, 0x0034
;   BEQL    T7, AT, 0x801109B8
*/
PATCH_START 0x80110890
    jal     Items_ShouldCheckItemUsabilityWhileSwimming_Hook
     lw     a0, 0x0080 (sp)
    beqzl   v0, .+0x120
PATCH_END
