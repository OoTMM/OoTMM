#include <combo.h>

/* Remove precond */
PATCH_START 0x80bc728c
  li t5,0xfff
PATCH_END

/* GI renumbering */
PATCH_START 0x80bc7500
  li a2,GI_MM_MASK_BREMEN
PATCH_END
