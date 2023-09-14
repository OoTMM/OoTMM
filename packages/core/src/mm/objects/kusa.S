#include <combo.h>

/* Kusa is both in object_kusa and gameplay_dangeon_keep */
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

/* Normal grass */
PATCH_TEXTURE 0x002, 0x78b8, 0x0a
PATCH_OBJECT 0x002, 0x7910
.quad 0
PATCH_END
