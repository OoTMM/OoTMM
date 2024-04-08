#include <combo.h>

/* Set the override flag */
DoorAna_SetOverride:
  li v0,1
  la v1,gIsEntranceOverride
  j 0x8009d94c
   sb v0,0(v1)

PATCH_START 0x80959cec
  jal DoorAna_SetOverride
PATCH_END
