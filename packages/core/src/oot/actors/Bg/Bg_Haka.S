#include <combo.h>

CheckGraveyardKid:
  /* Displaced check */
  lh at,0xa4(a2)
  li v0,0x53
  bne v0,at,CheckGraveyardKid_Ret
   nop

  /* Check for the soul of the kid */
  HOOK_SAVE
  jal comboHasSoulOot
   li a0,GI_OOT_SOUL_NPC_BOMBERS
  HOOK_RESTORE_KEEP_V0
  beqz v0,CheckGraveyardKid_Ret
   nop
  li v0,0x53

CheckGraveyardKid_Ret:
  jr ra
   nop

PATCH_START 0x8095b540
  jal CheckGraveyardKid
   nop
PATCH_END
