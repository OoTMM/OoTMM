#include <combo.h>

/* Check for All-Night Mask via the variable */
PATCH_START 0x80ae3c04
  li at,0x13
PATCH_END

PATCH_START 0x80ae3c18
  lh t1,0x1c(t0)
PATCH_END
