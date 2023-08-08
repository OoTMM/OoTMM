#include <combo.h>

/* Tsubo is both in object_tsubo and gameplay_dangeon_keep */
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

/* Grass bush */
PATCH_TEXTURE 0x0002, 0xb9e8, 0x0a
PATCH_OBJECT 0x0002, 0xba40
.quad 0
PATCH_END
