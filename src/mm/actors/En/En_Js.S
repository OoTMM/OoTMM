#include <combo.h>

/* Do not remove fierce deity */
PATCH_START 0x80968938
  nop
PATCH_END

/* Remove check for fierce deity */
PATCH_START 0x8096a654
  li t7,0xfff
PATCH_END
