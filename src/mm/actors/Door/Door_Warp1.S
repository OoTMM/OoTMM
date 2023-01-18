#include <combo.h>

#define BOSS_FLAGS_ADDR (gSaveContext + 0xf8 + 0x1c * 1 + 0x14)

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
PATCH_START 0x808b8d44
  lui a0,%HI(BOSS_FLAGS_ADDR)
PATCH_END

PATCH_START 0x808b8d6c
  lui a0,%HI(BOSS_FLAGS_ADDR)
PATCH_END

PATCH_START 0x808b8d50
  addiu a0,%LO(BOSS_FLAGS_ADDR)
PATCH_END

PATCH_START 0x808b8d80
  addiu a0,%LO(BOSS_FLAGS_ADDR)
PATCH_END

PATCH_START 0x808b8d60
  lw v0,0(a0)
PATCH_END

PATCH_START 0x808b8d84
  lw v0,0(a0)
PATCH_END

PATCH_START 0x808b8da0
  lw v0,0(a0)
PATCH_END

PATCH_START 0x808b8dbc
  lw v0,0(a0)
PATCH_END

/* Skip boss shortcut cutscene */
PATCH_START 0x808bab2c
  li t8,1
PATCH_END

PATCH_START 0x808b8fc8
  li t2,1
PATCH_END
