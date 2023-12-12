#include <combo.h>

/* Disable precond */
PATCH_START 0x80bfaacc
  li at,0xfff
PATCH_END

/* GI renumbering */
PATCH_START 0x80bfa9ac
  li a2,GI_MM_MASK_KAMARO
PATCH_END
