#include <combo.h>

/* Debug screen on crash */
PATCH_START 0x800af360
  jal osWritebackDCacheAll
PATCH_END
