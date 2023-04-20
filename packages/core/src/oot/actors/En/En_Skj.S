#include <combo.h>

/* Make the heart piece the first reward from the memory game */
PATCH_START 0x80a6fcf4
.int GI_OOT_HEART_PIECE
.int GI_OOT_RUPEE_GREEN
.int GI_OOT_RUPEE_BLUE
.int GI_OOT_RUPEE_RED
PATCH_END

/* Disable "SOLD OUT" */
PATCH_START 0x80a6eab0
  nop
PATCH_END
