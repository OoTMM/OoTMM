#include <combo.h>

/* Don't turn into child immediately */
PATCH_START 0x80b0a7c0
  nop
PATCH_END

/* Fast cutscenes */
PATCH_START 0x80b06ad8
  nop
PATCH_END

PATCH_START 0x80b0690c
  nop
PATCH_END

PATCH_START 0x80b07a30
  b .+0x10
PATCH_END
