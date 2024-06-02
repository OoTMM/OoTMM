#include <combo.h>

/* Turn invisible rupees into wonder items */
.macro WONDER_RUPEE slot, flag
PATCH_VROM 0x1fd9000 + 0x54 + \slot * 0x10
.short 0x02af
PATCH_END

PATCH_VROM 0x1fd9000 + 0x54 + \slot * 0x10 + 0xe
.short ((\flag << 2) | 0x01)
PATCH_END
.endm

WONDER_RUPEE 0x00, 0x01
WONDER_RUPEE 0x01, 0x02
