#include <combo.h>

/* Amy blocks - fast push */
PATCH_START 0x809444b0
.int 0x40800000
PATCH_END

PATCH_START 0x809437ec
  li t3,1
PATCH_END
