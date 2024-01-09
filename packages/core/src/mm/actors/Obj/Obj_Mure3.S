#include <combo.h>

/* Resize */
PATCH_START 0x8098f72c
.int 0x16c
PATCH_END

/* Reloc */
PATCH_START 0x8098f7c4
.int 0
PATCH_END

/* Use our init handler */
PATCH_START 0x8098f568
  jal ObjMure3_InitHijack
PATCH_END

/* Use our post spawn handler */
PATCH_START 0x8098f654
  jal ObjMure3_SetPostSpawnHandler
PATCH_END

PATCH_GROUP PG_MM_SHUFFLE_FREE_RUPEES

/* Disable flag */
PATCH_START 0x8098f544
  li v0,0
PATCH_END

PATCH_GROUP_END
