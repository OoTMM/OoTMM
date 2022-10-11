#include <combo.h>

/* Hide the Master Sword */
PATCH_START 0x80918f7c
  nop
  nop
PATCH_END

/* Use our handler */
PATCH_START 0x8091902c
  j BgTokiSwd_Handler
   nop
PATCH_END
