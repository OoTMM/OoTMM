#include <combo.h>

/* Disable hot water cooling in ER */
PATCH_GROUP PG_ER

PATCH_START 0x8010eba0
  jr ra
   nop
PATCH_END

PATCH_GROUP_END
