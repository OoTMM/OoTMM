#include <combo.h>

RupeeColor_Trampoline:
  HOOK_SAVE
  jal Hud_RupeeIconColor
    nop
  sw v0,0x40(sp)
  HOOK_RESTORE

  jr ra
   nop

/* TODO: Redo this hack without a deep hook - can probably be made ABI-safe by using the jal above */
PATCH_START 0x80075804
  addiu   t8,s1,0x08
  sw      t8,0x2b0(s4)
  jal     RupeeColor_Trampoline
   sw     t9,0x000(s1)
  lhu     t4,0x252(s7)
  move    at,v0
PATCH_END

/* Interpret zero as full transparent */
CheckColorZero:
  bnez at,CheckColorZero_End
    nop
  li t5,0

CheckColorZero_End:
  or t6,t5,at
  jr ra
   sw t6,0x4(s1)

PATCH_START 0x80075824
  jal CheckColorZero
   nop
PATCH_END
