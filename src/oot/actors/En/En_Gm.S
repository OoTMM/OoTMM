#include <combo.h>

/* Disable Medigoron intro text */
PATCH_START 0x80a9fbbc
  nop
PATCH_END

PATCH_START 0x80a9fbe4
  nop
PATCH_END
