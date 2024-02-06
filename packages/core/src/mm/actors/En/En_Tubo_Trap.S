#include <combo.h>

/* Resize the actor */
PATCH_START 0x80931448
.int 0x1ac
PATCH_END

/* Wrap the init function */
PATCH_START 0x8093144c
.int EnTuboTrap_InitWrapper
PATCH_END

/* Reloc */
PATCH_START 0x80931540
.int 0
PATCH_END

PATCH_START 0x8093089c
  j EnTuboTrap_SpawnShuffledDrop
   nop
PATCH_END
