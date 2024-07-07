#include <combo.h>

/* Do not use the extended Object_GetSlot when loading this actor, as it scans for multiple objects */
PATCH_START 0x80b742b0
  jal _Object_GetSlot
PATCH_END
