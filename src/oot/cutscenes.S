#include <combo/asm.h>

/* Disable the saria LW cutscene */
PATCH_START(0x80056e80)
  li v0, 1
PATCH_END
