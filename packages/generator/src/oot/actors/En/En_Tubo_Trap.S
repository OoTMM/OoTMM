#include <combo.h>

/* Resize */
PATCH_START 0x80a786f8
.int 0x1a4
PATCH_END

/* Hook the init func */
PATCH_START 0x80a786fc
.int EnTuboTrap_InitWrapper
PATCH_END

/* Relocs */
PATCH_START 0x80a787b8
.int 0
PATCH_END

PATCH_START 0x80a77be0
  j EnTuboTrap_SpawnShuffledDrop
   nop
PATCH_END
