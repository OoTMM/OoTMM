#include <combo.h>

/* Use our own destroy handler */
PATCH_START 0x80b36538
  nop
  nop
  jal     EnGg_MaybeDestroy
   move   a0,s0
  li      a1,0
  nop
PATCH_END
