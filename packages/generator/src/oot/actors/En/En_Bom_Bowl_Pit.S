#include <combo.h>

/* Disable healing when getting what used to be the heart piece */
PATCH_START 0x80aaf1dc
  nop
PATCH_END
