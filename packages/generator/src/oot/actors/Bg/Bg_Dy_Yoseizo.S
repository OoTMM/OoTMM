#include <combo.h>

/* Skip Magic check */
PATCH_START 0x808ebb1c
  li t6,1
PATCH_END

PATCH_START 0x808ebb5c
  li t2,1
PATCH_END
