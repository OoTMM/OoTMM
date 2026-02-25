#include <combo.h>

PATCH_START 0x8009c064
  jal DrawHUDWrapper
PATCH_END

RupeeCountLoopHook:
  la    t6,gHudRupeesBuffer
  addu  t6,s0
  jr ra
   lbu   t8,0(t6)

PATCH_START 0x80075cac
  li s0,0
PATCH_END

PATCH_START 0x80075cb8
  jal RupeeCountLoopHook
   nop
  nop
PATCH_END
