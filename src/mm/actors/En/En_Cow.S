#include <combo.h>

/* Disable bottle checks */
PATCH_START 0x8099cf28
  li v0,1
PATCH_END

PATCH_START 0x8099cca0
  li v0,1
PATCH_END

/* Disable talk */
PATCH_START 0x8099ce74
  nop
PATCH_END

PATCH_START 0x8099cd74
  nop
PATCH_END

/* Disable text boxes */
PATCH_START 0x8099ccb4
  nop
PATCH_END

PATCH_START 0x8099cc88
  nop
  nop
PATCH_END

PATCH_START 0x8099cc98
  nop
  nop
PATCH_END

PATCH_START 0x8099cbec
  nop
  nop
PATCH_END

PATCH_START 0x8099cbfc
  nop
  nop
PATCH_END
