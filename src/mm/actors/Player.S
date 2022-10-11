#include <combo.h>

/* Expand the GI buffer */
PATCH_START 0x808420c8
  li a0,0x6000
PATCH_END
