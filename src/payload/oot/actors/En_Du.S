#include <combo/asm.h>

/* Disable the cutscene itself */
PATCH_START 0x80953e80
  nop
PATCH_END

PATCH_START 0x80953e8c
  nop
PATCH_END

/* Skip dialogue */
PATCH_START 0x809544dc
  nop
PATCH_END
