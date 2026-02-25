#include <combo.h>

/* Lock the Gohma door */
PATCH_VROM 0x2eb4000 + 0x7e
.short 0x00ff
PATCH_END
