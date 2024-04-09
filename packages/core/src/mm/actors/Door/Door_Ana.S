#include <combo.h>

/* Set the override flag */
DoorAna_SetOverride:
  li v0,1
  la v1,gIsEntranceOverride
  j ActorCutscene_GetCurrentIndex
   sb v0,0(v1)

PATCH_START 0x808e05d8
  jal DoorAna_SetOverride
PATCH_END
