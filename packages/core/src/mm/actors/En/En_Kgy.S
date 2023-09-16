#include <combo.h>

/* Disable item patch/ */
PATCH_START 0x80b421dc
  nop
PATCH_END

PATCH_START 0x80b42200
  nop
  nop
  nop
  nop
PATCH_END

/* Give the razor blade/gilded sword immediately */
PATCH_START 0x80b421a8
  li t2,0xc50
PATCH_END

/* Disable dialogue after getting the item */
PATCH_START 0x80b41cbc
  jr ra
   nop
PATCH_END

/* Use our after kick handler */
PATCH_START 0x80b41ac4
  j EnKgy_AfterKick
PATCH_END
