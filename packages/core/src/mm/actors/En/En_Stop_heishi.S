#include <combo.h>

/* Make guards always let you pass */
PATCH_START 0x80ae80a8
  li t1,1
PATCH_END
