#include <combo.h>

/* Use our init wrapper */
PATCH_START 0x80a66cbc
.int EnWonderItem_InitWrapper
PATCH_END

PATCH_START 0x80a66e28
.int 0
PATCH_END
