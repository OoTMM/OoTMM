#include <combo.h>

/* Patch DrawGI */
PATCH_START 0x80aabb1c
  li a1,(DRAWGI_MM_MASK_DEKU - 1)
PATCH_END
