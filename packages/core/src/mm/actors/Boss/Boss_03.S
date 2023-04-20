#include <combo.h>

#define BOSS_SPAWN_FLAGS_ADDR (gMmExtraBoss + 1)
/* Gyorg */

/* Fix the spawn flag */
PATCH_START 0x809e2f84
  lui s3,%HI(BOSS_SPAWN_FLAGS_ADDR)
PATCH_END

PATCH_START 0x809e2f98
  nop
PATCH_END

PATCH_START 0x809e2fb0
  lbu t6,%LO(BOSS_SPAWN_FLAGS_ADDR)(s3)
PATCH_END

PATCH_START 0x809e2fbc
  andi t7,t6,0x4
PATCH_END

/* Skip the cutscene */
PATCH_START 0x809e33cc
  li t6,1
PATCH_END
