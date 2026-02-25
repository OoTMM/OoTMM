#include <combo.h>

/* Bypass the custom GetObject */
PATCH_START 0x808679c8
  jal _Object_GetSlot
PATCH_END

/* Use our own "door locked" handler */
PATCH_START 0x80867c1c
  move a1,a2
PATCH_END

PATCH_START 0x80867c2c
  jal comboDoorIsUnlocked
   nop
PATCH_END

Door_DrawDoorLock_Hook:
  j     Door_DrawDoorLock
   move a1, s0

PATCH_START 0x808685b0
  jal Door_DrawDoorLock_Hook
PATCH_END
