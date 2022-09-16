#include <combo/asm.h>

/* Skip DOM2_ADDR2 init */
PATCH_START 0x800914ac
  nop
PATCH_END
