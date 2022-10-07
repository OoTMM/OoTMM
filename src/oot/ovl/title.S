#include <combo.h>

.global hookDeep_Title_Main
hookDeep_Title_Main:
  HOOK_CALL(hook_Title_Main)

  /* Replaced code */
  lw v1,0x0(s0)
  lw v0,0x2c0(v1)

  /* Jump back */
  jr      ra
   nop

PATCH_START(0x808006a0)
  jal hookDeep_Title_Main
   nop
PATCH_END
