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

/* Check for boss beaten flag to enable warp pad */
PATCH_GROUP PG_MM_BOSS_WARP_PADS_BEATEN
PATCH_START 0x808b8d44
  lui a0,%HI(gMmExtraBoss)
PATCH_END

PATCH_START 0x808b8d6c
  lui a0,%HI(gMmExtraBoss)
PATCH_END

PATCH_START 0x808b8d50
  addiu a0,%LO(gMmExtraBoss)
PATCH_END

PATCH_START 0x808b8d80
  addiu a0,%LO(gMmExtraBoss)
PATCH_END

PATCH_START 0x808b8d60
  lbu v0,0(a0)
PATCH_END

PATCH_START 0x808b8d84
  lbu v0,0(a0)
PATCH_END

PATCH_START 0x808b8da0
  lbu v0,0(a0)
PATCH_END

PATCH_START 0x808b8dbc
  lbu v0,0(a0)
PATCH_END
PATCH_GROUP_END

/* Skip boss shortcut cutscene */
PATCH_START 0x808bab2c
  li t8,1
PATCH_END

PATCH_START 0x808b8fc8
  li t2,1
PATCH_END

/* Patch the boss warp pads */
PATCH_START 0x808b9410
  nop
PATCH_END
