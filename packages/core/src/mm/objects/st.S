#include <combo.h>

PATCH_OBJECT 0x020, 0x42e8
.int 0xde000000
.int 0x0a000000
.int 0
.int 0
PATCH_END

PATCH_OBJECT 0x020, 0x4580
.int 0xde000000
.int 0x0a000000
.int 0
.int 0
PATCH_END
