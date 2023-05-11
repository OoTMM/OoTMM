#include <combo.h>

/* Fix for FD link */
PATCH_START 0x80c039e0
  nop
PATCH_END
