#include <combo.h>

/* Ignore magic flag */
PATCH_START 0x80a618a8
  li t2,1
PATCH_END

PATCH_START 0x80a619a4
  li t7,1
PATCH_END

PATCH_START 0x80a62480
  li t2,1
PATCH_END

PATCH_START 0x80a62614
  li t6,1
PATCH_END

/* Do not run the map update */
PATCH_START 0x80a62ed0
  jr ra
   nop
PATCH_END

/* Relocs */
PATCH_START 0x80a64284
.int 0
PATCH_END
