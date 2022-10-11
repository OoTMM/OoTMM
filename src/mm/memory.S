#include <combo.h>

/* Use our own memory clearing to avoid overwriting the payload. */
PATCH_START(0x80080238)
  jal ClearHiMemory
PATCH_END
