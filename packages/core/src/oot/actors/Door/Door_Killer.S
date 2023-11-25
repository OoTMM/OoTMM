#include <combo.h>

/* Do not use the extended GetObjectSlot when loading this actor, as it scans for multiple objects */
PATCH_START 0x80b742b0
  jal GetObjectSlot
PATCH_END
