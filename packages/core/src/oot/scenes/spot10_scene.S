#include <combo.h>

PATCH_GROUP PG_OOT_LOST_WOODS_EXITS

// File VROM = 0x02146000
// Surface Type List Offset = 0x394
// sizeof(SurfaceType) = 8
// Type 3 = North Exits
// Type 6 = South Exits
// Type 17 = West Exits
// Type 22 = East Exits
// Exit Index Offset = 2
// Exit Index Mask = 0x1f

// Change exit values for Surface Types 3, 6, 17 and 22

PATCH_VROM 0x02146000 + 0x394 + (8 * 3) + 2
.byte 0x03
PATCH_END

PATCH_VROM 0x02146000 + 0x394 + (8 * 6) + 2
.byte 0x05
PATCH_END

PATCH_VROM 0x02146000 + 0x394 + (8 * 17) + 2
.byte 0x06
PATCH_END

PATCH_VROM 0x02146000 + 0x394 + (8 * 22) + 2
.byte 0x04
PATCH_END

// Change Surface Type 5 to be the same as Surface Type 6 except the exit index

PATCH_VROM 0x02146000 + 0x394 + (8 * 5)
.int 0x00000103
.int 0x000c3fca
PATCH_END

// Change Polygons that use Surface Type 5 to instead use Surface Type 1,
// which is the same except the Echo is 7 instead of 3.
// The polygons that are affected are Polygons 81 through 86 and 158 through 163,
// which are in the first room on the roof above the Northern and Western exits.

// Polygons List Offset = 0x49c
// sizeof(CollisionPoly) = 0x10
// Type offset = 0

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 81)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 82)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 83)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 84)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 85)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 86)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 158)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 159)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 160)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 161)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 162)
.short 0x0001
PATCH_END

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 163)
.short 0x0001
PATCH_END

// Change Polygons 121, which exits to Kokiri Forest to use Surface Type 5

PATCH_VROM 0x02146000 + 0x49c + (0x10 * 121)
.short 0x0005
PATCH_END

PATCH_GROUP_END
