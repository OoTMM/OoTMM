#include <combo.h>

/* Resize */
PATCH_START 0x80c25b3c
.int 0x1a0
PATCH_END

/* Use our init function */
PATCH_START 0x80c25b40
.int EnInvisibleRupee_InitWrapper
PATCH_END

PATCH_START 0x80c25bb0
.int 0
PATCH_END
