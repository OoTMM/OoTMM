#include <combo/asm.h>

/* Replace ItemEtcetera_Init */
PATCH_START(0x80a5dfcc)
  j hookItemEtcetera_Init
   nop
PATCH_END
