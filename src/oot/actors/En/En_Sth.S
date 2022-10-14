#include <combo.h>

/* The wallets are progressive in vanilla - disable that so we can identify the correct check */
PATCH_START 0x80b28240
  nop
PATCH_END

PATCH_START 0x80b28250
  nop
PATCH_END
