#include <combo.h>

/* Force quiver2 as the 2nd reward */
PATCH_START 0x80a90198
  move v0,at
PATCH_END

PATCH_START 0x80a90260
  move v0,at
PATCH_END
