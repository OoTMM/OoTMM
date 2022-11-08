#include <combo.h>

/* Hook into out menu handler */
KaleidoScope_AfterSetCutsorColor_Trampoline:
  lw      a0,0x40(sp)
  lw      s0,0x14(sp)
  lw      s1,0x18(sp)
  j       KaleidoScope_AfterSetCutsorColor
   addiu  sp,0x40

PATCH_START 0x8081c674
  j KaleidoScope_AfterSetCutsorColor_Trampoline
   nop
PATCH_END
