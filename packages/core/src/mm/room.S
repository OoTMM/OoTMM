#include <combo.h>

/* Hook OnRoomChange */
PATCH_START 0x801c26ac
.int OnRoomChange
PATCH_END
