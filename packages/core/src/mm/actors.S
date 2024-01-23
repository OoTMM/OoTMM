#include <combo.h>

/* Handle rooms with no enemies because of souls */
PATCH_START 0x800babdc
  jal SetRoomClearHook
PATCH_END

/*
Replaces:
  li        t5, 0x1
  or        a0, s1, zero
  beq       t4, zero, 0x800ba67c
   addiu    a2, s1, 0x1d40
  sb        t5, 0x1cab (s1)
  jal       Actor_DrawLensActors
   lbu      a1, 0x1caf (s1)
*/
PATCH_START 0x800ba660
  jal       Actor_AfterDrawAll
   or       a0, s1, zero
  nop
  nop
  nop
  nop
  nop
PATCH_END
