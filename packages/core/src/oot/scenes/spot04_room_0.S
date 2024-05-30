#include <combo.h>

/* Turn invisible rupees into wonder items */
.macro WONDER_RUPEE slot, flag
PATCH_VROM 0x2081000 + 0x94 + \slot * 0x10
.short 0x0112
PATCH_END

PATCH_VROM 0x2081000 + 0x94 + \slot * 0x10 + 0xc
.short 0x0001
.short (0x1200 | \flag)
PATCH_END
.endm

WONDER_RUPEE 0x01, 0x24
WONDER_RUPEE 0x02, 0x27
WONDER_RUPEE 0x03, 0x25
WONDER_RUPEE 0x04, 0x26

/* Move one wonder item on the platforms */
PATCH_VROM 0x2081000 + 0x94 + 38 * 0x10 + 2
.short 370
.short 5
.short -180
PATCH_END
