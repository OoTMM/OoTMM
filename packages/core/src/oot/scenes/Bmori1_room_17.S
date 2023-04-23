#include <combo.h>

/* Move switches */

PATCH_VROM 0x2486000 + 0xa8
.short 0xfcf4
PATCH_END

PATCH_VROM 0x2486000 + 0xc8
.short 0xfcf4
PATCH_END

PATCH_VROM 0x2486000 + 0xe8
.short 0xfcf4
PATCH_END
