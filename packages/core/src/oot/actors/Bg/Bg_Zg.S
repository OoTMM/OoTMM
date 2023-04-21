#include <combo.h>

/* Fix the gates not opening on some emulators */
PATCH_START 0x80afc388
  nop
  nop
PATCH_END
