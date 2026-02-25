#include <combo.h>

#define BOSS_SPAWN_FLAGS_ADDR (gMmExtraBoss + 1)
/* Goht */

/* Fix the spawn flag */
PATCH_START 0x80b05474
  lui t7,%HI(BOSS_SPAWN_FLAGS_ADDR)
  lbu t7,%LO(BOSS_SPAWN_FLAGS_ADDR)(t7)
PATCH_END

PATCH_START 0x80b05484
  andi t8,t7,0x2
PATCH_END

/* Skip the first cutscene */
PATCH_START 0x80b055f8
  li t7,1
PATCH_END
