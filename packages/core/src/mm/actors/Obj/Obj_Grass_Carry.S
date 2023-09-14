#include <combo.h>

/* Use our drop handler */
PATCH_START 0x809ab80c
  jal ObjGrassCarry_SpawnDropsWrapper
   move a0,s2
PATCH_END

PATCH_START 0x809abd60
.int 0
PATCH_END
