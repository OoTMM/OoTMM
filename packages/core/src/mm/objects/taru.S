#include <combo.h>

.macro PATCH_TEXTURE object, offset, seg
PATCH_OBJECT \object, \offset
.quad 0xde00000000000000 | (\seg << 24)
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
.quad 0x0
PATCH_END
.endm

PATCH_TEXTURE OBJECT_TARU, 0x450, 0x08
PATCH_TEXTURE OBJECT_TARU, 0x4f0, 0x08
PATCH_TEXTURE OBJECT_TARU, 0x580, 0x08
PATCH_TEXTURE OBJECT_TARU, 0x610, 0x09
