#include <combo.h>

/* DrawGI patch */
PATCH_START 0x80ba3460
  li a1,(DRAWGI_MM_MASK_SUN - 1)
PATCH_END

/* Reloc for sakon door */
PATCH_START 0x80ba35b0
.int 0
PATCH_END

/* Telescope ER */
PATCH_GROUP PG_ER_TELESCOPES

PATCH_START 0x80ba3344
  j ObjNozoki_TelescopeER
   nop
PATCH_END

PATCH_GROUP_END
