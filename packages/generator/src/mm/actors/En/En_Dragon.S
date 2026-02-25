#include <combo.h>

// Allow Pinnacle Rock Sea Snakes to take damage from the hookshot
PATCH_START 0x80b605fb
.byte 0xf1
PATCH_END
