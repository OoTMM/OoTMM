#include <combo.h>

#define ROOM_09   0x2122000

PATCH_GROUP PG_MM_JP_LAYOUT_STONE_TOWER_TEMPLE

/* Remove sun switch */
PATCH_VROM ROOM_09 + 0x188
.short 0xffff
PATCH_END

PATCH_GROUP_END
