#include <combo.h>

/* Skip the button check */
PATCH_START 0x800af360
  jal osWritebackDCacheAll
PATCH_END
