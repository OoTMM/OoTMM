#include <combo.h>

/* Remove zora mask check */
PATCH_START 0x80a44f1c
  nop
  nop
PATCH_END

/* Remove spider tokens interaction */
PATCH_START 0x80a44d00
  nop
PATCH_END
