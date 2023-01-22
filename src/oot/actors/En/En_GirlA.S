#include <combo.h>

EnGirlA_AfterInit2_Trampoline:
  move    a0,s0
  lw      s0,0x18(sp)
  j       EnGirlA_AfterInit2
   addiu  sp,0x30

PATCH_START 0x808647dc
  j EnGirlA_AfterInit2_Trampoline
   nop
PATCH_END
