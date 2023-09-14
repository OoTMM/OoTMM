#include <combo.h>

/* Forward the index */
PATCH_START 0x80a5bd60
  move a2,v1
PATCH_END

PATCH_START 0x80a5ed48
.int 0
PATCH_END

/* Disable fading out */
PATCH_START 0x80a5e80c
  jr ra
   nop
PATCH_END
