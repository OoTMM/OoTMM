#include <combo.h>

/* Remove check for garo mask */
PATCH_START 0x808f532c
  nop
PATCH_END
