#include <combo.h>

/* Make the Poe Collector give the item after a single big poe */
PATCH_START 0x80b6b2ac
  li  at,100
PATCH_END
