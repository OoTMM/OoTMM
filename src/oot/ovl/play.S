#include <combo.h>

/* Patch Play_Init */
PATCH_START 0x800f13e8
  .long hookPlay_Init
PATCH_END

/* hook Play_Draw */
PATCH_START 0x8009ac90
  lui t1,%HI(Play_DrawWrapper)
PATCH_END

PATCH_START 0x8009ac98
  addiu t1,%LO(Play_DrawWrapper)
PATCH_END
