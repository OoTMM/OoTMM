#include <combo.h>

/* Enable ER */
EnWarpTag_SetERFlag:
  li v0,1
  la v1,gIsEntranceOverride
  j 0x8019f128
   sb v0,0(v1)

PATCH_START 0x809c0ee0
  jal EnWarpTag_SetERFlag
PATCH_END
