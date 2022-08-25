#include <combo/asm.h>

/* Expand the GI buffer size to 0x4000 */
PATCH_START(0x80844f7c)
  li a0,0x4008
PATCH_END
