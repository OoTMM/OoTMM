#include <combo.h>

PATCH_GROUP PG_OOT_CHEST_GAME_SHUFFLE

/* Don't reset the key count */
PATCH_START 0x80b19030
  nop
PATCH_END

PATCH_GROUP_END
