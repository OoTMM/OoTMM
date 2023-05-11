#include <combo.h>

/* Sticks */
AdultStick:
  lw      t8,(gSaveContext+0x04)
  bnez    t8,.LAdultStickRet
   nop
  la      t2,kDListEmpty
.LAdultStickRet:
  addiu   t8,v1,0x0008
  jr      ra
   sw     t8,0x02c0(t7)

PATCH_START 0x8007b8b4
  jal AdultStick
   nop
PATCH_END

/* Masks */
AdultMasks:
  lw      t7,(gSaveContext + 0x04)
  bnez    t7,.LAdultMasksRet
   nop
  la      t6,kDListEmpty
.LAdultMasksRet:
  sw      t6,0x04(v0)
  jr      ra
   lb     t7,0x013f(s0)

PATCH_START 0x808483dc
  jal AdultMasks
   nop
PATCH_END
