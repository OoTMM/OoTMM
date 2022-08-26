#include <combo/asm.h>

/* Set the required cucco count to 1 */
PATCH_START 0x80a9e6d0
  slti at,t6,1
PATCH_END

/* Skip reward dialogue */
PATCH_START 0x80a9e900
  nop
PATCH_END

PATCH_START 0x80a9e910
  nop
PATCH_END
