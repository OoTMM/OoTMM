#include <combo.h>

/* Make guards always let you pass */
PATCH_START 0x80ae80a8
  li t1,1
PATCH_END

/* Make guards handle Fierce Deity as Zora */
PATCH_START 0x80ae8024
  bnel  v0,at,. + 0xb0
  li    a2,1
PATCH_END
