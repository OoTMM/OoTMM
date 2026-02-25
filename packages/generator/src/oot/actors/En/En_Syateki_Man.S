#include <combo.h>

/* Force quiver as a reward */
PATCH_START 0x809a12cc
  move at,v0
PATCH_END
