#include <combo/asm.h>

.global hookDeep_En_Mag_Main
hookDeep_En_Mag_Main:
  /* Prologue */
  addiu sp,-0xc
  sw    ra,0x8(sp)
  sw    a0,0x4(sp)
  sw    a1,0x0(sp)

  /* Apply our changes */
  jal   hook_En_Mag_Main

  /* Restore */
  lw      a1,0x0(sp)
  lw      a0,0x4(sp)
  lw      ra,0x8(sp)
  addiu   sp,0xc

  /* Original code */
  sw a1,0x44(sp)
  lw a0,0x44(sp)

  /* Return */
  jr      ra
   nop

/* Replaces
   sw a1,0x44(sp)
   lw a0,0x44(sp)
*/
PATCH_START(0x80aed38c)
  jal hookDeep_En_Mag_Main
   nop
PATCH_END
