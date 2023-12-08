#include <combo.h>

/* GI renumbering */
PATCH_START 0x80bbee58
  li a2,GI_MM_POWDER_KEG
PATCH_END

PATCH_START 0x80bbef14
  li a2,GI_MM_POWDER_KEG
PATCH_END
