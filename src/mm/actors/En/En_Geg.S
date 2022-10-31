#include <combo.h>

/* Skip a check for the mask */
PATCH_START 0x80bb31e0
  li t7,0xff
PATCH_END
