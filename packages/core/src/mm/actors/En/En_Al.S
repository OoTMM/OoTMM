#include <combo.h>

/* Replace the check for Kafei's mask */
PATCH_START 0x80bdeb28
  la  t8,(gMmExtraFlags2) /* Extra Flags 2 */
  lui at,0x1000 /* 0x10000000: Kafei mask bit */
  lw  t8,0(t8)
  and t1,t8,at
  nop
PATCH_END
