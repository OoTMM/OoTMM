#include <combo.h>

/* Resize the actor */
PATCH_START 0x80a80adc
.int 0x198
PATCH_END

/* Use our init wrapper */
PATCH_START 0x80a80ae0
.int EnKusa_InitWrapper
PATCH_END

/* Use our draw func */
PATCH_START 0x80a80150
  lui t8,%HI(EnKusa_DrawWrapper)
PATCH_END

PATCH_START 0x80a80158
  addiu t8,%LO(EnKusa_DrawWrapper)
PATCH_END

/* Relocs */
PATCH_START 0x80a80d78
.int 0
PATCH_END

PATCH_START 0x80a80c4c
.int 0
.int 0
PATCH_END

PATCH_GROUP PG_OOT_SHUFFLE_GRASS

PATCH_START 0x80a8022c
  jal EnKusa_SpawnShuffledDrop
PATCH_END

PATCH_START 0x80a8055c
  jal EnKusa_SpawnShuffledDrop
PATCH_END

/* Relocs */
PATCH_START 0x80a80c68
.int 0
PATCH_END

PATCH_START 0x80a80ccc
.int 0
PATCH_END

PATCH_GROUP_END
