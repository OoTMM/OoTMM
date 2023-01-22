#include <combo.h>

/* Door of time cutscene skip */
PATCH_START 0x80930c34
  li a0,EV_OOT_CHK_DOOR_TIME
PATCH_END

PATCH_START 0x80930c3c
  jal GetEventChk
PATCH_END
