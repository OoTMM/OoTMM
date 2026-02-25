#include <combo.h>

/* Disable goron check */
PATCH_START 0x808d1528
  li t8,1
PATCH_END

PATCH_START 0x808d18d8
  nop
PATCH_END

/* Disable DC clear check for bombchu */
PATCH_START 0x808cf67c
  li t3,1
PATCH_END

/* Override first textbox */
PATCH_START 0x808d04fc
  jal Shop_DisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d05e8
  jal Shop_DisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d0b5c
  jal Shop_DisplayTextBox
   move a1,t5
PATCH_END

PATCH_START 0x808d0d1c
  jal Shop_DisplayTextBox
   move a1,t5
PATCH_END

PATCH_START 0x808d164c
  jal Shop_DisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d1720
  jal Shop_DisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d17f4
  jal Shop_DisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d1888
  jal Shop_DisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d197c
  jal Shop_DisplayTextBox
   move a1,t1
PATCH_END

PATCH_START 0x808d1bd8
  jal Shop_DisplayTextBox
   move a1,t0
PATCH_END

PATCH_START 0x808d1c7c
  jal Shop_DisplayTextBox
   move a1,t4
PATCH_END

PATCH_START 0x808d1a24
  jal Shop_DisplayTextBox
   move a1,t4
PATCH_END

PATCH_START 0x808d1b58
  jal Shop_DisplayTextBox
   move a1,t2
PATCH_END

PATCH_START 0x808cf944
  jal Shop_DisplayTextBox
   move a1,t2
PATCH_END

/* Override the second textbox */
PATCH_START 0x808d08b4
  jal Shop_DisplayTextBoxConfirm
   move a1,t6
PATCH_END

/* Don't set the flag for bombchu shop */
PATCH_START 0x808640c0
  li t6,0
PATCH_END
