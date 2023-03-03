#include <combo.h>

EnGirlA_AfterHandler_Trampoline:
  /* girlA is in s0 */
  move a0,s0

  /* play is on the stack */
  lw a1,0x34(sp)

  /* Replaced code and jump  */
  lw      s0,0x18(sp)
  j       EnGirlA_AfterHandler
   addiu  sp,0x30

/* Hook after EnGirlA_Handler */
PATCH_START 0x808647dc
  j EnGirlA_AfterHandler_Trampoline
   nop
PATCH_END
