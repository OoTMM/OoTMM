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

/* Normal grass */
PATCH_TEXTURE 0x002, 0xb9e8, 0x0a
PATCH_OBJECT 0x002, 0xba40
.quad 0
PATCH_END

/* Alt grass */
PATCH_TEXTURE 0x12b, 0x0158, 0x0a
PATCH_TEXTURE 0x12b, 0x02f8, 0x0a
PATCH_OBJECT 0x12b, 0x01b0
.quad 0
PATCH_END
PATCH_OBJECT 0x12b, 0x0350
.quad 0
PATCH_END
