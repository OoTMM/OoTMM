#include <combo.h>

/* Use the same ocarina check as Dm_Stk */
PATCH_START 0x80aaaf8c
  jal DmStk_OcarinaCheck
   nop
  nop
PATCH_END

/* Use our draw hook */
PATCH_START 0x80aab334
  move a0,s1
  jal DmChar02_DrawOcarina
   move a1,s0
PATCH_END

PATCH_START 0x80aab370
  nop
PATCH_END

/* Do not set link's form */
PATCH_START 0x80aab210
  nop
PATCH_END
