#include <combo.h>

/* Fix for FD link (?) */
PATCH_VROM 0x26FC000 + 0x8FF6
.short 0x00a9
PATCH_END

PATCH_VROM 0x26FC000 + 0x8FFC
.short 0x00a9
PATCH_END
