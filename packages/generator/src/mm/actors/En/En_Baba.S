#include <combo.h>

/* Remove item check */
PATCH_START 0x80ba8b88
  li t5,0xfff
PATCH_END

/* GI renumbering */
PATCH_START 0x80ba9b04
  li a2,GI_MM_MASK_BLAST
PATCH_END
