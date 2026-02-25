#include <combo.h>

/* Resize */
PATCH_START 0x80be21dc
.int 0x19c
PATCH_END

/* Use our init wrapper */
PATCH_START 0x80be21e0
.int EnHitTag_InitWrapper
PATCH_END

PATCH_START 0x80be2244
.int 0
PATCH_END

/* Use our update wrapper */
PATCH_START 0x80be21e8
.int EnHitTag_Update
PATCH_END

PATCH_START 0x80be224c
.int 0
PATCH_END
