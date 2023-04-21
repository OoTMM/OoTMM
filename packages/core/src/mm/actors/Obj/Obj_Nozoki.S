#include <combo.h>

/* DrawGI patch */
PATCH_START 0x80ba3460
  li a1,(DRAWGI_MM_MASK_SUN - 1)
PATCH_END
