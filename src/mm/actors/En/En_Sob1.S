#include <combo.h>

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
