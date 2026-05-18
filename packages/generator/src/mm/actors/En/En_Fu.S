#include <combo.h>

/* GI renumbering */
PATCH_START 0x809635b0
  li a2,GI_MM_HEART_PIECE
PATCH_END

PATCH_START 0x809635e4
  li a2,GI_MM_RUPEE_PURPLE
PATCH_END

EnFu_CanPlayDay1_Hook:
  /* Original code */
  or    a0,a3,zero
  srlv  t2,t9,t1
  HOOK_CALL_RET EnFu_CanPlayDay1
  jr      ra
   nop

PATCH_START 0x80963a50
  jal EnFu_CanPlayDay1_Hook
   nop
  bnez v0, .+0x1c
PATCH_END

EnFu_CanPlayDay2_Hook:
  /* Original code */
  and   t6,t4,t5
  srlv  t8,t6,t7
  HOOK_CALL_RET EnFu_CanPlayDay2
  jr      ra
   nop

PATCH_START 0x80963b38
  jal EnFu_CanPlayDay2_Hook
   nop
  bnez v0, .+0x1c
PATCH_END
