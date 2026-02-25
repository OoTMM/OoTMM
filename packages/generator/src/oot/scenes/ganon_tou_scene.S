#include <combo.h>

/* Move the spawn point near the castle entrance */
PATCH_VROM 0x0292b000 + 0x86
.short 0x0291
PATCH_END

PATCH_VROM 0x0292b000 + 0x8e
.short 0x0dff
PATCH_END
