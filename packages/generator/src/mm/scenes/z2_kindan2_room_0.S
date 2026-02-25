#include <combo.h>

/* Set the Ssh object ID */
PATCH_VROM 0x024b2000 + 0x54
.short 0x0127
PATCH_END
