#include <combo.h>

/* Don't turn into child immediately */
PATCH_START 0x80b0a7c0
  nop
PATCH_END
