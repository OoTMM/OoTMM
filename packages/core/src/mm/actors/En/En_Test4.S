#include <combo.h>

/* Skip the clock tower cutscene */
PATCH_START 0x80a42ebc
  nop
PATCH_END

PATCH_START 0x80a42ed8
  nop
PATCH_END

PATCH_START 0x80a42e04
.word 0x00000000
.half 0x1000
PATCH_END

PATCH_START 0x80A427DC
  nop
PATCH_END
