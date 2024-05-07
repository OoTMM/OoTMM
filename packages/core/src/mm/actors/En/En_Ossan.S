#include <combo.h>

/* Override first textbox */
PATCH_START 0x808a8688
  jal Shop_DisplayTextBox
   move a1,t2
PATCH_END

PATCH_START 0x808a94cc
  jal Shop_DisplayTextBox
   move a1,t2
PATCH_END

PATCH_START 0x808a95c8
  jal Shop_DisplayTextBox
   move a1,t2
PATCH_END

PATCH_START 0x808a9a94
  jal Shop_DisplayTextBox
   move a1,t6
PATCH_END

PATCH_START 0x808a9c68
  jal Shop_DisplayTextBox
   move a1,t6
PATCH_END

PATCH_START 0x808aa194
  jal Shop_DisplayTextBox
   move a1,t1
PATCH_END

PATCH_START 0x808aa20c
  jal Shop_DisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808aa2ac
  jal Shop_DisplayTextBox
   move a1,t4
PATCH_END

/* Override the second textbox */
PATCH_START 0x808a9888
  jal Shop_DisplayTextBoxConfirm
   move a1,t6
PATCH_END
