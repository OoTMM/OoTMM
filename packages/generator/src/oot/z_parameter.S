#include <combo.h>

/* Lens Magic Meter Consumption */
/*
Replaces:
  NOP
  LB      T7, 0x0033 (V1)
  ADDIU   T9, R0, 0x0050
  ADDIU   T8, T8, 0xFFFF
  SB      T8, 0x0033 (V1)
  SH      T9, 0x0230 (V0)
  LUI     RA, 0x800F
  LH      RA, 0x75B4 (RA)
  LUI     V0, 0x800F
  LUI     T7, 0x800F
  SLL     RA, RA, 1
  ADDU    V0, V0, RA
  LH      V0, 0x75A8 (V0)
  ADDIU   T7, T7, 0x7590
*/
PATCH_START 0x80072E00
  li      t9,0x50
  sh      t9,0x230(v0)
  jal     Magic_Decrement
   nop
  beqz    v0,.+0x2CC
   nop
  jal     comboIsChateauActive
   nop
  bnez    v0, .+0x2BC
   nop
  jal     Magic_FlashMeterBorder
   nop
  b       .+0x2AC
   nop
PATCH_END

Interface_CustomIsRestrictionCategoryAll_Hook:
  HOOK_CALL_RET Interface_CustomIsRestrictionCategoryAll
  jr ra
   nop

PATCH_START 0x8006f0ec
  addiu   a1, a1, 0x0001
  jal     Interface_CustomIsRestrictionCategoryAll_Hook
   lbu    a0, 0x0068 (v1)
  beqz    v0, .+0x74
   addiu  a0, zero, 0x10
  lbu     v0, 0x0068 (v1)
  lw      t7, 0x0038 (sp)
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END

Interface_CustomIsRestrictionCategoryAll2_Hook:
  HOOK_CALL_RET Interface_CustomIsRestrictionCategoryAll2
  jr ra
   nop

PATCH_START 0x8006f190
  addiu   a1, a1, 0x0001
  jal     Interface_CustomIsRestrictionCategoryAll2_Hook
   lbu    a0, 0x0068 (v1)
  beqz    v0, .+0x6c
   nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END

Interface_CustomIsRestrictionCategoryBottle_Hook:
  HOOK_CALL_RET Interface_CustomIsRestrictionCategoryBottle
  jr ra
   nop

PATCH_START 0x8006ecc0
  addiu   a1, a1, 0x0001
  jal     Interface_CustomIsRestrictionCategoryBottle_Hook
   lbu    a0, 0x0068 (v1)
  beqz    v0, .+0x2c
   nop
  nop
  nop
  nop
  nop
PATCH_END

PATCH_START 0x8006ed18
  addiu   a1, a1, 0x0001
  jal     Interface_CustomIsRestrictionCategoryBottle_Hook
   lbu    a0, 0x0068 (v1)
  beqz    v0, .+0x2c
   nop
  nop
  nop
  nop
  nop
PATCH_END
