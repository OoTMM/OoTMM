#include <combo.h>

/* Use our hook */
PATCH_START 0x80b23f60
  j EnGe1_AfterCaught
PATCH_END

PATCH_START 0x80b24094
  j EnGe1_AfterCaught
PATCH_END
