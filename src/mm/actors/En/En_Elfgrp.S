#include <combo.h>

/* Pretend we're Deku to trigger the hint message */
PATCH_START 0x80a3a9b0
  li t1,3
PATCH_END
