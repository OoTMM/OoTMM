#include <combo.h>

/* Faster Terminal Field Scrub fly path + cutscene QoLs */
PATCH_START 0x80BCB17C
.half 0x1000
PATCH_END

PATCH_START 0x80BCBBA0
  nop
PATCH_END

PATCH_START 0x80BCC204
  nop
PATCH_END

/* GI renumbering */
PATCH_START 0x80bcb960
  li a2,GI_MM_HEART_PIECE
PATCH_END
