#include <combo.h>

/* Don't give the bossa nova */
PATCH_START 0x80959ee0
  nop
PATCH_END

/* Extend activation range for new wave check */
PATCH_START 0x80959f70
  lui at, 0x4380
PATCH_END
