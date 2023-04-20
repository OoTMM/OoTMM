#include <combo.h>

/* Remove the check for the notebook */
PATCH_START 0x809c4fa4
  li t1,0
PATCH_END
