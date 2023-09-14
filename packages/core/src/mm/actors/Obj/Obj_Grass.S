#include <combo.h>

/* Resize the actor */
PATCH_START 0x809aab0c
.int 0x3368
PATCH_END

/* Disable the initial dlist */
PATCH_START 0x809aa9f0
/*.int 0xdf000000
.int 0*/
PATCH_END

/* Hook deep inside the rendering */
PATCH_START 0x809aa704
  jal ObjGrass_DrawBush
   addiu a0,s2,0xc
  nop
  nop
  nop
  nop
  nop
  nop
  nop
PATCH_END

/* Hook into grass drops */
PATCH_START 0x809a97e8
  jal ObjGrass_SpawnBushDropWrapper
PATCH_END

PATCH_START 0x809aac30
.int 0
PATCH_END

PATCH_GROUP PG_MM_SHUFFLE_GRASS

/* Disable grass fade-in */
PATCH_START 0x809aa798
  jr ra
   nop
PATCH_END

PATCH_GROUP_END
