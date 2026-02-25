#include <combo.h>

PATCH_GROUP PG_ER_WALLMASTERS

EnWallmas_TakePlayerEr_Hook:
  j       EnWallmas_TakePlayerEr
   or     a1, s0, zero

PATCH_START 0x808758ac
  jal     EnWallmas_TakePlayerEr_Hook
PATCH_END

PATCH_GROUP_END
