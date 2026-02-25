#include <combo.h>

/* Skip the anju + kafei cutscene */
PATCH_START 0x80a40034
  li v0,1
PATCH_END

/* Hook after update */
EnTest3_AfterUpdate_Trampoline:
  move    a0,s0
  move    a1,s1
  lw      s0,0x18(sp)
  lw      s1,0x1c(sp)
  lw      s2,0x20(sp)
  j       EnTest3_AfterUpdate
   addiu  sp,0x48

PATCH_START 0x80a40cdc
  j EnTest3_AfterUpdate_Trampoline
   nop
PATCH_END
