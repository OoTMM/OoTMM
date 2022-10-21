#include <combo.h>

/* Disable the rainbow bridge cutscene */
PATCH_START 0x80aab5bc
  jr ra
   nop
PATCH_END
