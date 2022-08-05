#include <combo/asm.h>

.global hookDeep_Title_Main
hookDeep_Title_Main:
  /* Prologue */
  addiu sp,-0x8
  sw    ra,0x4(sp)
  sw    a0,0x0(sp)

  /* Apply our changes */
  jal   hook_Title_Main
   nop

  /* Replaced code */
  lw v1,0x0(s0)
  lw v0,0x2c0(v1)

  /* Jump back */
  lw      a0,0x0(sp)
  lw      ra,0x4(sp)
  jr      ra
   addiu  sp,0x8

PATCH_START(0x808006a0)
  jal hookDeep_Title_Main
   nop
PATCH_END
