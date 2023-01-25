#include <combo.h>

/* Disable goron check */
PATCH_START 0x808d1528
  li t8,1
PATCH_END
