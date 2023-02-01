#include <combo.h>

#define BOSS_SPAWN_FLAGS_ADDR (gSaveContext + 0xf8 + 0x1c * 1 + 0x14 + 1)
/* Twinmold */

/* Fix the spawn flag */
PATCH_START 0x809da5c8
  lui t6,%HI(BOSS_SPAWN_FLAGS_ADDR)
  lbu t6,%LO(BOSS_SPAWN_FLAGS_ADDR)(t6)
  andi t7,t6,0x8
PATCH_END

PATCH_START 0x809deb74
  lui t3,%HI(BOSS_SPAWN_FLAGS_ADDR)
  lbu t3,%LO(BOSS_SPAWN_FLAGS_ADDR)(t3)
PATCH_END

PATCH_START 0x809deb80
  andi t4,t3,0x8
PATCH_END
