#include <combo.h>

/* Disable royal tomb cutscene */
PATCH_START 0x80a8747c
  nop
PATCH_END

/* Disable windmill cutscene */
PATCH_START 0x80a873b0
  nop
PATCH_END
