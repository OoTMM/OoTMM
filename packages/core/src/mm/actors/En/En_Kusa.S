#include <combo.h>

/* Resize the actor */
PATCH_START 0x809366c4
.int 0x1a4
PATCH_END

/* Patch the init func */
PATCH_START 0x809366c8
.int EnKusa_InitWrapper
PATCH_END

/* Reloc */
PATCH_START 0x80936ab4
.int 0
PATCH_END

/* Patch the draw func */
PATCH_START 0x8093593c
  lui t1,%HI(EnKusa_DrawWrapper)
  lui t0,%HI(EnKusa_DrawWrapper)
PATCH_END

PATCH_START 0x80935948
  addiu t1,%LO(EnKusa_DrawWrapper)
  addiu t0,%LO(EnKusa_DrawWrapper)
PATCH_END

/* Relocs */
PATCH_START 0x80936950
.int 0
.int 0
.int 0
.int 0
PATCH_END

PATCH_GROUP PG_MM_SHUFFLE_GRASS

/* Patch drops */
PATCH_START 0x80935a28
  jal EnKusa_SpawnShuffledDrop
PATCH_END

PATCH_START 0x80935dfc
  jal EnKusa_SpawnShuffledDrop
PATCH_END

/* Relocs */
PATCH_START 0x80936970
.int 0
PATCH_END

PATCH_START 0x809369cc
.int 0
PATCH_END

PATCH_GROUP_END
