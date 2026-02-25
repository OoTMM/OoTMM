#include <combo.h>

/* Tsubo is both in object_tsubo and gameplay_dangeon_keep */
/* racetsubo is also similar */
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
PATCH_TEXTURE 0x0f9, 0x17D8, 0x0a
PATCH_TEXTURE 0x0f9, 0x18C8, 0x0b

/* gameplay_dangeon_keep */
PATCH_TEXTURE 0x003, 0x17EB8, 0x0a
PATCH_TEXTURE 0x003, 0x17FD8, 0x0a
PATCH_TEXTURE 0x003, 0x17F78, 0x0b

/* object_racetsubo */
PATCH_TEXTURE 0x19c, 0x318, 0x0a
PATCH_TEXTURE 0x19c, 0x2A8, 0x0b
