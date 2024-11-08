#include <combo.h>

/* Remove fire temple rocks as child */
PATCH_GROUP PG_OOT_FIRE_TEMPLE_CHILD
PATCH_VROM 0x0225e000 + 0x228
.short 0xffff
PATCH_END

PATCH_VROM 0x0225e000 + 0x238
.short 0xffff
PATCH_END

PATCH_VROM 0x0225e000 + 0x248
.short 0xffff
PATCH_END
PATCH_GROUP_END
