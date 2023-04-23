#include <combo.h>

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

/* Override the first textbox */
PATCH_START 0x80a0d238
  jal comboShopDisplayTextBox
   move a1,t2
PATCH_END

PATCH_START 0x80a0e18c
  jal comboShopDisplayTextBox
   move a1,t2
PATCH_END

PATCH_START 0x80a0e3fc
  jal comboShopDisplayTextBox
   move a1,t5
PATCH_END

PATCH_START 0x80a0e950
  jal comboShopDisplayTextBox
   move a1,t1
PATCH_END

PATCH_START 0x80a0e9c8
  jal comboShopDisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x80a0ea68
  jal comboShopDisplayTextBox
   move a1,t4
PATCH_END

/* Override the second textbox */
PATCH_START 0x80a0e2d4
  jal comboShopDisplayTextBoxConfirm
   move a1,t6
PATCH_END

/* Fix for FD link */
PATCH_START 0x80a10ac0
.int 0x80a0ca84
.int 0x80a0ca84
PATCH_END
