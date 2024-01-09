#include <combo.h>

/* Resize */
PATCH_START 0x80b5584c
.int 0x164
PATCH_END

/* Reloc */
PATCH_START 0x80b558e4
.int 0
PATCH_END

/* Use our init handler */
PATCH_START 0x80b55684
  jal ObjMure3_InitHijack
PATCH_END

/* Use our post spawn handler */
PATCH_START 0x80b55770
  jal ObjMure3_SetPostSpawnHandler
PATCH_END

PATCH_GROUP PG_OOT_SHUFFLE_FREE_RUPEES

/* Disable flag */
PATCH_START 0x80b55660
  li v0,0
PATCH_END

PATCH_GROUP_END
