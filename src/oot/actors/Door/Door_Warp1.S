#include <combo.h>

.lcomm sDoorWarp1Actor, 4

/* Hook after draw */
Hook_DoorWarp1_SaveActor:
  /* Save s1 */
  la a2,sDoorWarp1Actor
  sw s1,0x00(a2)

  /* Replaced code */
  sw  s0,0x34(sp)
  jr ra
   lw t6,0x9c(a1)

PATCH_START 0x8090735c
  jal Hook_DoorWarp1_SaveActor
   nop
PATCH_END

Trampoline_DoorWarp1_AfterDrawWarp:
  la  at,sDoorWarp1Actor
  lw  a0,0x00(at)
  lw  a1,0xe4(sp)
  lw  s1,0x38(sp)
  j DoorWarp1_AfterDrawWarp
   addiu  sp,0xe0

PATCH_START 0x80908180
  j Trampoline_DoorWarp1_AfterDrawWarp
   nop
PATCH_END

/* Skip Shadow Cutscene */
PATCH_START 0x80906784
  li t4,1
PATCH_END

/* Skip Spirit Cutscene */
PATCH_START 0x809066f4
  li t5,1
PATCH_END
