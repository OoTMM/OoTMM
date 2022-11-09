#include <combo.h>

/* Patch DoorWarp1_RemainsIndex */
PATCH_START 0x808b849c
  jr ra
   li v0,0
PATCH_END

/* Use our draw hook */
DoorWarp1_AfterDraw_Trampoline:
  lw      a0,0x18(sp)
  lw      a1,0x1c(sp)
  lw      ra,0x14(sp)
  j       DoorWarp1_AfterDraw
   addiu  sp,0x18

PATCH_START 0x808bb8a4
  j DoorWarp1_AfterDraw_Trampoline
   nop
PATCH_END
