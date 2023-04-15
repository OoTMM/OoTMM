#include <combo.h>

.global hookDeep_FileChoose_Update
hookDeep_FileChoose_Update:
  HOOK_CALL(hook_FileChoose_Update)

  /* Original code */
  lh  t8,0x4484(a3)
  sll t5,t8,0x2

  /* Jump back */
  jr      ra
   nop

PATCH_START(0x80813594)
  jal hookDeep_FileChoose_Update
   nop
PATCH_END

/* Mute menu BGM */
PATCH_START(0x80813dd0)
  nop
PATCH_END
