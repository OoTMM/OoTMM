#include <combo/asm.h>

.global hookDeep_Title_Main
hookDeep_Title_Main:
  /* Original prologue */
  addiu sp,-0x8
  sw    ra,0x8(sp)
  sw    a0,0x4(sp)

  /* Apply our changes */
  jal   hook_Title_Main

  /* Replaced code */
  lw v1,0x0(s0)
  lw v0,0x2c0(v1)

  /* Jump back */
  lw      a0,0x4(sp)
  lw      ra,0x8(sp)
  jr      ra
   addiu  sp,0x8

  /* Hijack the stack to store a0 and ra */
  /* and jump to the patched function */
  /*sw    ra,0x20(sp)*/
  /*jal   hook_Title_Main*/
   /*sw   a0,0x1c(sp)*/

  /* Restore and continue */
  /*lw    ra,0x20(sp)
  lw    a0,0x1c(sp)
  jr    ra
   sw   s0,0x18(sp)*/

/* Replaces
     lw v1,0x0(s0)
     lw v0,0x2c0(v1)
*/
PATCH_START(0x808006a0)
  jal hookDeep_Title_Main
   nop
PATCH_END
