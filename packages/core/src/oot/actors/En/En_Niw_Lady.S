#include <combo.h>

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

/* Adult - Skip text for pocket egg */
PATCH_START 0x80a9ec10
  nop
  nop
PATCH_END

PATCH_START 0x80a9ec20
  nop
PATCH_END

PATCH_START 0x80a9ec2c
  li v0,0
PATCH_END

/* Adult - Skip text for cojiro */
PATCH_START 0x80a9ed64
  nop
  nop
PATCH_END

PATCH_START 0x80a9ed74
  nop
PATCH_END

PATCH_START 0x80a9ed80
  li v0,0
PATCH_END
