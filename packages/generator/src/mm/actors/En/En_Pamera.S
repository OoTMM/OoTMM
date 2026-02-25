#include <combo.h>

/* Set the ER flag when kicked out of Music House */
EnPamera_SetEntranceShuffleFlag:
  li a0,1
  lui a1,%HI(gIsEntranceOverride)
  jr ra
   sb a0,%LO(gIsEntranceOverride)(a1)

PATCH_START 0x80bd9cb0
  j EnPamera_SetEntranceShuffleFlag
PATCH_END
