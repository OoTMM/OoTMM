#include <combo.h>

PATCH_START 0x80aaf814
  lw  a0,0x24(sp)
  jal DmChar08_Emerge
PATCH_END
