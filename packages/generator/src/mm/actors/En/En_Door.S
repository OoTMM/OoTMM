#include <combo.h>

/* Patch the lock drawing */
PATCH_START 0x808675ac
  nop
  nop
  jal EnDoor_DrawLock
   move a1,s0
PATCH_END
