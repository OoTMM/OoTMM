#include <combo.h>

/* Override the scrub spawn flag */
/* Actually part of En_Shopnuts */
PATCH_START 0x80b40134
  nop
PATCH_END

PATCH_START 0x80b40140
  li t0,1
PATCH_END
