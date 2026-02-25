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

/* object tsubo */
PATCH_TEXTURE 0x12c, 0x17D8, 0x0a
PATCH_TEXTURE 0x12c, 0x18C8, 0x0b

/* gameplay_dangeon_keep */
PATCH_TEXTURE 0x003, 0x17888, 0x0a
PATCH_TEXTURE 0x003, 0x179A8, 0x0a
PATCH_TEXTURE 0x003, 0x17948, 0x0b
