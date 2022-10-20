#include <combo.h>

/* Remove a check for the biggoron sword flag */
PATCH_START 0x80b57b68
  li t6,0
PATCH_END
