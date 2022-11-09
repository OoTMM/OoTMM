#include <combo.h>

/* Patch the MM melody model */
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

NOP1 0xb70,  3
NOP1 0xb70,  4
NOP1 0xb70,  5
NOP1 0xb70,  6
NOP1 0xbe0,  0
NOP1 0xbe0,  1
NOP1 0xbe0,  2
NOP1 0xbe0,  3
NOP1 0xbe0,  4
NOP1 0xbe0, 11
NOP1 0xc48,  0
NOP1 0xc48,  1
NOP1 0xc48,  2
NOP1 0xc48,  3
NOP1 0xc88,  4
NOP1 0xc88,  5
NOP1 0xc88,  6
NOP1 0xcc8,  1
NOP1 0xcc8,  0
NOP1 0xcc8,  2
NOP1 0xb70,  2
NOP1 0xb70,  1
NOP1 0xb70,  0
NOP2 0xcc8,  1
NOP2 0xcc8,  0
NOP2 0xc88,  6
NOP2 0xc88,  4
NOP2 0xc88,  5
NOP2 0xc88,  3
NOP2 0xc48,  3
NOP2 0xc48,  2
NOP2 0xc48,  1
NOP2 0xc48,  0
NOP2 0xbe0, 11
NOP2 0xbe0, 10
NOP2 0xbe0,  4
NOP2 0xbe0,  3
NOP2 0xbe0,  2
NOP2 0xbe0,  1
NOP2 0xbe0,  0
NOP2 0xb70, 12
NOP2 0xb70,  5
NOP2 0xb70,  4
NOP2 0xb70,  3
NOP2 0xb70,  2
NOP2 0xb70,  1
