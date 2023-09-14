#include <combo.h>

/* Override size */
PATCH_START 0x809abbbc
.int 0x1a8
PATCH_END

/* Use our own init */
PATCH_START 0x809abbc0
.int ObjGrassCarry_InitWrapper
PATCH_END

/* Relocs */
PATCH_START 0x809abdd0
.int 0
PATCH_END

/* Use our drop handler */
PATCH_START 0x809ab80c
  jal ObjGrassCarry_SpawnDropsWrapper
   move a0,s2
PATCH_END

PATCH_START 0x809abd60
.int 0
PATCH_END
