#include <combo/asm.h>

/* Expand the GI buffer size to 0x8000 */
PATCH_START(0x80844f7c)
  li a0,0x8008
PATCH_END
