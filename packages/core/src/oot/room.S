#include <combo.h>

/* Hook OnRoomChange */
PATCH_START 0x800f8f74
.int OnRoomChange
PATCH_END
