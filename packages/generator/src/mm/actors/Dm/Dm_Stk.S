#include <combo.h>

/* Use a flag instead of Ocarina check */
.globl DmStk_OcarinaCheck
DmStk_OcarinaCheck:
  la   t6,(gMmExtraFlags2) /* Extra Flags 2 */
  lw   t6,0(t6)
  not  t6
  li   at,0x4000 /* Ocarina flag */
  jr   ra
   and t6,at

PATCH_START 0x80aa13d8
  jal DmStk_OcarinaCheck
   nop
  nop
PATCH_END
