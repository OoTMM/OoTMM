#include <combo.h>

Ocarina_CheckCustomSongs_Hook:
  addiu   sp,-0x18
  sw      ra,0x14(sp)
  sw      a0,0x18(sp)

  HOOK_CALL Ocarina_CheckCustomSongs

  lw      v0,0x18(sp)

  /* Displaced code */
  lbu     t3,0x80121f42
  lbu     t5,0x80121f43
  lui     s0,0x8012
  lui     t1,0x8010

  lw        ra,0x14(sp)
  jr        ra
   addiu    sp,0x18

PATCH_START 0x800c2860
  move    a0, ra
  jal     Ocarina_CheckCustomSongs_Hook
   nop
  move    ra,v0
PATCH_END

/* Call our button masking code */
PATCH_START 0x800c1f78
  j OcarinaMaskButtons
PATCH_END
