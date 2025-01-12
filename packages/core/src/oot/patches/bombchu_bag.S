#include <combo.h>

/* Limit the number of bombchus */

/* Item_Give */
PATCH_START 0x80070c1c
  lui t5,%HI(gMaxBombchuOot)
PATCH_END

PATCH_START 0x80070c30
  lbu t5,%LO(gMaxBombchuOot)(t5)
  slt at,t7,t5
PATCH_END

PATCH_START 0x80070cc0
  lui t7,%HI(gMaxBombchuOot)
PATCH_END

PATCH_START 0x80070ccc
  lbu t7,%LO(gMaxBombchuOot)(t7)
  slt at,t9,t7
PATCH_END

InterfaceBombchuAmmoColorHook:
  lbu at,(gMaxBombchuOot)
  beq t0,at,InterfaceBombchuAmmoColorHookJump
   nop
  j 0x80074d88
   nop
InterfaceBombchuAmmoColorHookJump:
  j 0x80074da0
   nop

PATCH_START 0x80074d84
  j InterfaceBombchuAmmoColorHook
PATCH_END
