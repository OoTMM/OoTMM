#include <combo.h>

/* Lock the boss door */
PATCH_VROM 0x02ea8000 + 0x7e
.short 0x0140
PATCH_END
