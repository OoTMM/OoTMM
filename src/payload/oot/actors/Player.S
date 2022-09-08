#include <combo/asm.h>

/* Expand the GI buffer size to 0x6000 */
PATCH_START(0x80844f7c)
  li a0,0x6008
PATCH_END
