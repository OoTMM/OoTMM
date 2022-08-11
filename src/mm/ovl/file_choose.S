#include <combo/asm.h>

.global hookDeep_FileChoose_Update
hookDeep_FileChoose_Update:
  /* Prologue */
  addiu sp,-0x14
  sw    ra,0x10(sp)
  sw    a0,0x0c(sp)
  sw    a1,0x08(sp)
  sw    a2,0x04(sp)
  sw    a3,0x00(sp)

  /* Apply our changes */
  jal   hook_FileChoose_Update
   nop

  /* Restore */
  lw      a3,0x00(sp)
  lw      a2,0x04(sp)
  lw      a1,0x08(sp)
  lw      a0,0x0c(sp)
  lw      ra,0x10(sp)
  addiu   sp,0x14

  /* Original code */
  lh  t8,0x4484(a3)
  sll t5,t8,0x2

  /* Jump back */
  jr      ra
   nop


PATCH_START(0x80813594)
  jal hookDeep_FileChoose_Update
   nop
PATCH_END

/* Mute menu BGM */
PATCH_START(0x80813dd0)
  nop
PATCH_END
