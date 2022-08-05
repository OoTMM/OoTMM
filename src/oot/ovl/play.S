#include <combo/asm.h>

/* Patch Play_Init */
PATCH_START(0x800f13e8)
  .long hookPlay_Init
PATCH_END
