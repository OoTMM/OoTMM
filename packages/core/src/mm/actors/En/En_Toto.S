#include <combo.h>

/* Don't give the item */
PATCH_START 0x80ba4e10
  nop
PATCH_END

/* Use our wrapper */
PATCH_START 0x80ba4fe8
.int EnToto_UpdateWrapper
PATCH_END

PATCH_START 0x80ba5348
.int 0
PATCH_END
