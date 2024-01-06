#include <combo.h>

/* GI renumbering */
PATCH_START 0x809635b0
  li a2,GI_MM_HEART_PIECE
PATCH_END

PATCH_START 0x809635e4
  li a2,GI_MM_RUPEE_PURPLE
PATCH_END

/* On day 1, allow bombchu bag too */
CheckBombchuBag:
  /* Original code */
  or    a0,a3,zero
  srlv  t2,t9,t1

  /* If the player has the bomb bag, return */
  bnez t2,CheckBombchuBag_Ret
   nop

  /* Check bombchu bag */
  lui   t9,%HI(gSaveContext + 0x70 + ITS_MM_BOMBCHU)
  lb    t9,%LO(gSaveContext + 0x70 + ITS_MM_BOMBCHU)(t9)
  xori  t9,ITEM_MM_BOMBCHU
  bnez  t9,CheckBombchuBag_Ret
   nop

  li t2,1

  /* We need to check the bombchu bag */
CheckBombchuBag_Ret:
  jr ra
   nop


PATCH_GROUP PG_MM_BOMBCHU_BAG

PATCH_START 0x80963a50
  jal CheckBombchuBag
   nop
PATCH_END

PATCH_GROUP_END
