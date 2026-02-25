#include <combo.h>

/* Make royal tomb explode outside of cutscene */
PATCH_START 0x8095a348
  la   t7,gSaveContext
  lhu  t7,(0xed4+(1 * 2))(t7)
  andi t7,0x2000
  nop
  nop
  nop
  li at,0x2000
PATCH_END
