#include <combo.h>

/* Resize the actor */
PATCH_START 0x8092951c
.int 0x1a0
PATCH_END

/* Wrap the init function */
PATCH_START 0x80929520
.int ObjTsubo_InitWrapper
PATCH_END

/* Relocs */
PATCH_START 0x809298c4
.int 0
PATCH_END
