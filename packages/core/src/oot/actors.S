#include <combo.h>

/* Handle rooms with no enemies because of souls */
PATCH_START 0x8002508c
  jal SetRoomClearHook
PATCH_END

/* Navi color for CSMC */
PATCH_START 0x8001fa60
  j   Actor_AfterSetNaviToActor
PATCH_END

/* Hook actor run update */
PATCH_START 0x800240d8
  jal Actor_RunUpdate
   move a2,t9
PATCH_END
