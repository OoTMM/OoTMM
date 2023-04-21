#include <combo.h>

.global hookDeep_FileChoose_Update
hookDeep_FileChoose_Update:
  HOOK_CALL(hook_FileChoose_Update)

  /* Original code */
  lh  t3,0x4a2e(a2)
  sll t4,t3,0x2

  /* Jump back */
  jr      ra
   nop

PATCH_START(0x80811ab8)
  jal hookDeep_FileChoose_Update
   nop
PATCH_END

/* Patch the FileChoose_Init function to mute BGM when coming from MM */
PATCH_START(0x808124a8)
  jal hook_FileChoose_Init
PATCH_END
