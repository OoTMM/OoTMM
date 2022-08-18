#include <combo/asm.h>

/* Hook LoadObjectGi */
PATCH_START(0x80838830)
  j hook_LoadObjectGi
   nop
PATCH_END
