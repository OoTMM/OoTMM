#include <combo.h>

/* Check for All-Night Mask via the variable */
PATCH_START 0x80ae3c04
  li at,0x13
PATCH_END

PATCH_START 0x80ae3c18
  lh t1,0x1c(t0)
PATCH_END

/* Override the first textbox */
PATCH_START 0x80ae26b4
  jal comboShopDisplayTextBox
   move a1,t2
PATCH_END

PATCH_START 0x80ae3ffc
  jal comboShopDisplayTextBox
   move a1,t3
PATCH_END

PATCH_START 0x80ae4104
  jal comboShopDisplayTextBox
   move a1,t7
PATCH_END

PATCH_START 0x80ae45a8
  jal comboShopDisplayTextBox
   move a1,t1
PATCH_END

PATCH_START 0x80ae4620
  jal comboShopDisplayTextBox
   move a1,t0
PATCH_END

/* Override the second textbox */
PATCH_START 0x80ae291c
  jal comboShopDisplayTextBoxConfirm
   move a1,t1
PATCH_END

/* Disable check for keaton mask */
PATCH_START 0x80ae1cec
  li t5,0xff
PATCH_END

/* Make it impossible to sell gold dust */
PATCH_START 0x801c210c
.short 0
PATCH_END

/* GI renumbering */
PATCH_START 0x80ae3b64
  li t0,GI_MM_RUPEE_BLUE
PATCH_END

PATCH_START 0x80ae3b38
  li t1,GI_MM_RUPEE_RED_10 /* ??? */
PATCH_END

PATCH_START 0x80ae3b44
  li t2,GI_MM_RUPEE_RED
PATCH_END

PATCH_START 0x80ae3b20
  li v1,GI_MM_RUPEE_PURPLE
PATCH_END

PATCH_START 0x80ae3b58
  li t3,GI_MM_RUPEE_GOLD
PATCH_END

PATCH_START 0x80ae1d00
  li t0,GI_MM_LETTER_TO_MAMA
PATCH_END

PATCH_START 0x80ae1d58
  li t0,GI_MM_MASK_KEATON
PATCH_END

PATCH_START 0x80ae1d88
  li t6,GI_MM_LETTER_TO_MAMA
PATCH_END
