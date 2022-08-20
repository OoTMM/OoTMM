#include <combo/asm.h>

#if 0
/* Replace ItemEtcetera_Init */
PATCH_START(0x80a5dfcc)
  j hookItemEtcetera_Init
   nop
PATCH_END


/* Replace ItemEtcetera_Update */
PATCH_START(0x80a5e594)
  j hookItemEtcetera_Update
   nop
PATCH_END
#endif
