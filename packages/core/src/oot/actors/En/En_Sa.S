#include <combo.h>

/* Disable a check for Saria's Song */
PATCH_START 0x80aa9548
  nop
  nop
PATCH_END
