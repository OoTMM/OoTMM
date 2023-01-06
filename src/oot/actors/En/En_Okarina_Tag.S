#include <combo.h>

/* Disable windmill cutscene */
PATCH_START 0x80a873b0
  nop
PATCH_END
