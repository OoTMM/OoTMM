#include <combo.h>

/* Disable first cutscene */
PATCH_START 0x800fbf60
.int 0x10
PATCH_END

PATCH_START 0x80090c58
li t9, 0x0
PATCH_END

/* Hook save creation to create MM save */
PATCH_START 0x80090eb0
  j comboCreateSaveMM
PATCH_END
