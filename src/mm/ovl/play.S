#include <combo.h>

/* Hook into Play_Init */
PATCH_START(0x801bd9b8)
.int hookPlay_Init
PATCH_END

/* Wrap Play_Draw */

PATCH_START 0x8016a8a8
  lui t4,%HI(Play_DrawWrapper)
PATCH_END

PATCH_START 0x8016a8b0
  addiu t4,%LO(Play_DrawWrapper)
PATCH_END
