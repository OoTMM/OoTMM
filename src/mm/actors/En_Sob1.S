#include <combo/asm.h>

EnSob1_AfterBuy_Trampoline:
  lw a0,0x20(sp)
  lw a1,0x24(sp)

  /* Replaced code */
  j EnSob1_AfterBuy
   addiu sp,0x20

/* Don't detach the actor yet */
PATCH_START 0x80a0eaa8
  nop
PATCH_END

PATCH_START 0x80a0eaec
  j EnSob1_AfterBuy_Trampoline
   nop
PATCH_END

/* Override the textbox */
PATCH_START 0x80a0e2d4
  jal EnSob1_DisplayTextBoxConfirm
   move a1,t6
PATCH_END
