#include <combo.h>

/* Use our init wrapper */
PATCH_START 0x80966330
.int EnMm_InitWrapper
PATCH_END

PATCH_START 0x809663fc
.int 0
PATCH_END
