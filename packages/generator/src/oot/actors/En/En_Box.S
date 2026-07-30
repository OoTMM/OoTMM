#include <combo.h>

/* Use our wrappers */
PATCH_START 0x8086a090
.int EnBox_InitWrapper
PATCH_END

PATCH_START 0x8086a09c
.int EnBox_DrawWrapper
PATCH_END

/* Disable some relocs */
PATCH_START 0x8086a26c
.int 0
PATCH_END

PATCH_START 0x8086a278
.int 0
PATCH_END

/* Disable the ice effect */
PATCH_START 0x80869884
  jr ra
   nop
PATCH_END

PATCH_GROUP PG_OOT_SKIP_CUTSCENES

// Reduce appearTimer
PATCH_START 0x8086928c
  nop
  nop
PATCH_END

PATCH_START 0x80869348
  nop
PATCH_END

PATCH_START 0x808693ac
  nop
PATCH_END

PATCH_START 0x80869204
  nop
PATCH_END

PATCH_GROUP_END
