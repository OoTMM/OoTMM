#include <combo.h>

/* Force Quiver2 as a reward */
PATCH_START 0x809a12b8
  li v0,1
PATCH_END
