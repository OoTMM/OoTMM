#include <combo.h>

#define DEKUCITY_SCENE   0x2534000
#define DEKUCITY_ROOM_01 0x2554000
#define DEKUCITY_ROOM_02 0x2563000

PATCH_GROUP PG_MM_JP_LAYOUT

/* Beans Grotto Spawn location */
PATCH_VROM DEKUCITY_SCENE + 0x90 + 0x10 * 9 + 2
.short 450
.short 80
.short 784
.short 0x7
.short 0x0
.short 0x7f
PATCH_END

/* Rupee circle */
PATCH_VROM DEKUCITY_ROOM_01 + 0x176
.short 405
.short 0
.short 1554
PATCH_END

PATCH_GROUP_END
