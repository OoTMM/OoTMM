#include <combo.h>

/* Disable first cutscene */
PATCH_START 0x800fbf60
.int 0x10
PATCH_END

PATCH_START 0x80090c58
li t9, 0x0
PATCH_END

/* Hook save creation to create saves */
PATCH_START 0x80090d48
  nop
PATCH_END

PATCH_START 0x80090d70
  nop
PATCH_END

PATCH_START 0x80090d84
  jal comboCreateSave
PATCH_END
