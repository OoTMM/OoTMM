#include <combo.h>

/* Cursed Ikana post-dungeon-clear link spawn flags */
PATCH_VROM 0x02036000 + 0x12E
.byte 0x02
PATCH_END

/* Cleared Ikana post-dungeon-clear link spawn flags */
PATCH_VROM 0x02036000 + 0x145C6
.byte 0x02
PATCH_END
