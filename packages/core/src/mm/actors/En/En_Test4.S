#include <combo.h>

/* Skip the clock tower cutscene */
PATCH_START 0x80a42ebc
  nop
PATCH_END

PATCH_START 0x80a42ed8
  nop
PATCH_END

PATCH_START 0x80a42e10
  li t3,0
PATCH_END

PATCH_START 0x80A427DC
  nop
PATCH_END
