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

/* Use our own mask-giving hook */
PATCH_START 0x80b35afc
  la at,EnGg_HandlerGiveItem
  sw at,0x1d4(s0)
  nop
  nop
PATCH_END
