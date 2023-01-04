#include <combo.h>

/* Disable filters on pictobox - for emu */
PATCH_START 0x80165e28
  nop
PATCH_END
