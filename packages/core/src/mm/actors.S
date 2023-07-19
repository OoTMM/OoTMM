#include <combo.h>

/* Handle rooms with no enemies because of souls */
PATCH_START 0x800babdc
  jal SetRoomClearHook
PATCH_END
