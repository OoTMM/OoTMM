#include <combo.h>

/* GI renumbering */
PATCH_START 0x808b3f3c
  li a2,GI_MM_STICK
PATCH_END

PATCH_START 0x808f21f8
  li a2,GI_MM_STICK
PATCH_END
