#include <combo.h>

#define BOSS_SPAWN_FLAGS_ADDR (gMmExtraBoss + 1)
/* Odolwa */

/* Fix the spawn flag */
PATCH_START 0x809d0e30
  lui t5,%HI(BOSS_SPAWN_FLAGS_ADDR)
PATCH_END

PATCH_START 0x809d0ea0
  lbu t5,%LO(BOSS_SPAWN_FLAGS_ADDR)(t5)
PATCH_END

PATCH_START 0x809d0eac
  andi t6,t5,0x1
PATCH_END

/* Skip cutscene */
PATCH_START 0x809d10c8
  li t6,1
PATCH_END
