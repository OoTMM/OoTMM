#include <combo.h>

/* Remove the check for the notebook */
PATCH_START 0x809c4fa4
  li t1,0
PATCH_END

/* GI renumbering */
PATCH_START 0x809c59b8
  li a2,GI_MM_BOMBER_NOTEBOOK
PATCH_END

PATCH_START 0x809c5a84
  li a2,GI_MM_BOMBER_NOTEBOOK
PATCH_END
