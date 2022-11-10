#include <combo.h>

/* Patch the MM melody model */
/* Horrible hacks */
.macro NOP addr, offset
PATCH_OBJECT 0x008f, \addr + \offset * 8
.quad 0
PATCH_END
.endm

.macro NOP1 addr, offset
PATCH_OBJECT 0x008f, \addr + \offset * 8 + 1
.byte 0
.byte 0
.byte 0
PATCH_END
.endm

.macro NOP2 addr, offset
PATCH_OBJECT 0x008f, \addr + \offset * 8 + 5
.byte 0
.byte 0
.byte 0
PATCH_END
.endm

.macro V addr, offset, x, y, z, tx, ty, nx, ny, nz
PATCH_OBJECT 0x008f, \addr + \offset * 0x10
.short \x
.short \y
.short \z
.short 0
.short \tx
.short \ty
.byte \nx
.byte \ny
.byte \nz
.byte 0xff
PATCH_END
.endm

V 0x90, 0, 11, 13, -3, 17, 39, 116, 225, 0
V 0x90, 1, 11, 13,  3, 14, 39, 116, 225, 0
V 0x90, 2, 15, 35,  3, 14, 36, 116, 225, 0
V 0x90, 3, 15, 35, -3, 17, 36, 116, 225, 0

/*NOP  0xb70,  2*/

NOP  0xb70,  1
NOP  0xb70,  3
NOP  0xb70,  4
NOP  0xb70,  5
NOP  0xbe0,  0
NOP  0xbe0,  1
NOP  0xbe0,  2
NOP  0xbe0,  3
NOP  0xbe0,  4
NOP  0xbe0, 11
NOP  0xc48,  0
NOP  0xc48,  1
NOP  0xc48,  2
NOP  0xc48,  3
NOP  0xc88,  4
NOP  0xc88,  5
NOP  0xc88,  6
NOP  0xcc8,  0
NOP  0xcc8,  1
NOP1 0xb70,  0
NOP1 0xb70,  6
NOP1 0xcc8,  2
NOP2 0xb70, 12
NOP2 0xbe0, 10
NOP2 0xc88,  3
