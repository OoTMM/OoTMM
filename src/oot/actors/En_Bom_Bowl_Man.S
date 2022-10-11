#include <combo.h>

/* Use event flag 0x22 to check for playable bowling */
PATCH_START 0x80aad8d0
  andi t8,t7,0x04
PATCH_END

PATCH_START 0x80aada44
  andi t4,t3,0x04
PATCH_END

/* Hook after the first handler */
PATCH_START 0x80aad6c0
  j EnBomBowlMan_AfterHandler
PATCH_END
