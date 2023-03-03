#include <combo.h>

/* Disable goron check */
PATCH_START 0x808d1528
  li t8,1
PATCH_END

/* Disable DC clear check for bombchu */
PATCH_START 0x808cf67c
  li t3,1
PATCH_END

/* Override first textbox */
PATCH_START 0x808d04fc
  jal comboShopDisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d05e8
  jal comboShopDisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d0b5c
  jal comboShopDisplayTextBox
   move a1,t5
PATCH_END

PATCH_START 0x808d0d1c
  jal comboShopDisplayTextBox
   move a1,t5
PATCH_END

PATCH_START 0x808d164c
  jal comboShopDisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d1720
  jal comboShopDisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d17f4
  jal comboShopDisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d1888
  jal comboShopDisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d197c
  jal comboShopDisplayTextBox
   move a1,t1
PATCH_END

PATCH_START 0x808d1bd8
  jal comboShopDisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d1c7c
  jal comboShopDisplayTextBox
   move a1,t4
PATCH_END

PATCH_START 0x808d1a24
  jal comboShopDisplayTextBox
   move a1,t4
PATCH_END

PATCH_START 0x808d1b58
  jal comboShopDisplayTextBox
   move a1,t2
PATCH_END

PATCH_START 0x808cf944
  jal comboShopDisplayTextBox
   move a1,t2
PATCH_END

/* Override the second textbox */
PATCH_START 0x808d08b4
  jal comboShopDisplayTextBoxConfirm
   move a1,t6
PATCH_END

/* Don't set the flag for bombchu shop */
PATCH_START 0x808640c0
  li t6,0
PATCH_END
