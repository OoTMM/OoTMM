#include <combo.h>

/* Set the entrance override flag */
SetEntranceOverride:
  lui a1,%HI(gIsEntranceOverride)
  li a2,1
  j 0x801306a4
   sb a2,%LO(gIsEntranceOverride)(a1)

PATCH_START 0x80899ddc
  jal SetEntranceOverride
PATCH_END
