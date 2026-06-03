#include <combo.h>

/* Patch the lock drawing */
PATCH_START 0x808675ac
  nop
  nop
  jal EnDoor_DrawLock
   move a1,s0
PATCH_END

/* Patch the door check */
PATCH_START 0x80866c2c
  move a0,a3
PATCH_END

PATCH_START 0x80866c48
  jal EnDoor_IsOpenInhibited
PATCH_END
