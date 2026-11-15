#include <combo.h>

EnemyMmSetupActorFlagHook:
  addiu sp,sp,-0x28
  sw ra,0x24(sp)
  sw a0,0x10(sp)
  sw a1,0x14(sp)
  sw v0,0x18(sp)
  sw v1,0x1c(sp)
  jal comboEnemyMmSetupActorFlag
   lhu a0,0x00(s0)
  move t0,v0
  lw a0,0x10(sp)
  lw a1,0x14(sp)
  lw v0,0x18(sp)
  lw v1,0x1c(sp)
  lw ra,0x24(sp)
  jr ra
   addiu sp,sp,0x28

PATCH_START 0x800b9420
  jal EnemyMmSetupActorFlagHook
   nop
PATCH_END
