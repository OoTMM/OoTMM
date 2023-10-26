#include <combo.h>

PATCH_GROUP PG_OOT_CHEST_GAME_SHUFFLE

/* Use our handler */
PATCH_START 0x80ac3510
  j EnChanger_Init
   nop
PATCH_END

PATCH_GROUP_END
