#include <combo.h>

/* Resize the ObjTsubo actor */
PATCH_START 0x80a6601c
.int 0x198
PATCH_END

/* Wrap the Tsubo ctor */
PATCH_START 0x80a66020
.int ObjTsubo_InitWrapper
PATCH_END

/* Draw */
PATCH_START 0x80a65a0c
  la t6,ObjTsubo_DrawWrapper
PATCH_END

/* Relocs */
PATCH_START 0x80a66140
.int 0
.int 0
PATCH_END

PATCH_START 0x80a661f4
.int 0
PATCH_END

PATCH_START 0x80a65220
  j ObjTsubo_SpawnShuffledDrop
   nop
PATCH_END
