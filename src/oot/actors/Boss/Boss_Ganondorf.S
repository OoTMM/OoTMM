#include <combo.h>

/* Fast boss intro */
PATCH_START 0x80910b58
  li t2,1
PATCH_END

PATCH_START 0x80910b78
  li t5,1
PATCH_END
