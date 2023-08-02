#include <combo.h>

/* Resize the ObjTsubo actor */
PATCH_START 0x80a6601c
.int 0x194
PATCH_END

/* Wrap the Tsubo ctor */
PATCH_START 0x80a66020
.int ObjTsubo_InitWrapper
PATCH_END

/* Relocs */
PATCH_START 0x80a661f4
.int 0
PATCH_END
