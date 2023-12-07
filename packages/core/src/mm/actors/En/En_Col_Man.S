#include <combo.h>

/* GI renumbering */
PATCH_START 0x80afdec4
  li a2,GI_MM_HEART_PIECE
PATCH_END

PATCH_START 0x80afdee0
  li a2,GI_MM_RECOVERY_HEART
PATCH_END
