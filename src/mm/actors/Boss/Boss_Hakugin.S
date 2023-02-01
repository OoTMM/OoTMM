#include <combo.h>

#define BOSS_SPAWN_FLAGS_ADDR (gSaveContext + 0xf8 + 0x1c * 1 + 0x14 + 1)
/* Goht */

/* Fix the spawn flag */
PATCH_START 0x80b05474
  lui t7,%HI(BOSS_SPAWN_FLAGS_ADDR)
  lbu t7,%LO(BOSS_SPAWN_FLAGS_ADDR)(t7)
PATCH_END

PATCH_START 0x80b05484
  andi t8,t7,0x2
PATCH_END
