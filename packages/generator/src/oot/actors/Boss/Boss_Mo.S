#include <combo.h>

/* Skip boss intro */
PATCH_START 0x809a6944
  li t0,1
PATCH_END
