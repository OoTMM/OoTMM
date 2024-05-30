#include <combo.h>

/* Resize */
PATCH_START 0x80a66cb8
.int 0x1c8
PATCH_END

/* Use our init wrapper */
PATCH_START 0x80a66cbc
.int EnWonderItem_InitWrapper
PATCH_END

PATCH_START 0x80a66e28
.int 0
PATCH_END

PATCH_START 0x80a66cc4
.int EnWonderItem_UpdateWrapper
PATCH_END

PATCH_START 0x80a66e30
.int 0
PATCH_END
