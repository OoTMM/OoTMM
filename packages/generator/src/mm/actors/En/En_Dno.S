#include <combo.h>

/* Disable precond */
PATCH_START 0x80a7298c
  li t3,0xfff
PATCH_END

/* GI renumbering */
PATCH_START 0x80a729a0
  li t4,GI_MM_RUPEE_RED
PATCH_END

PATCH_START 0x80a72994
  li t5,GI_MM_MASK_SCENTS
PATCH_END
