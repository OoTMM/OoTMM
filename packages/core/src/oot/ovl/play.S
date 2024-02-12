#include <combo.h>

/* Patch Play_Init */
PATCH_START 0x800f13e8
  .long hookPlay_Init
PATCH_END

/* hook Play_Update */
PATCH_START 0x8009ac90
  lui t1,%HI(Play_UpdateWrapper)
PATCH_END

PATCH_START 0x8009ac98
  addiu t1,%LO(Play_UpdateWrapper)
PATCH_END

/* Transition hooks */
Play_TransitionDone_Hook:
  sw      zero, 0x0098 (s0)
  HOOK_SAVE
  jal     Play_TransitionDone
   or     a0, s0, zero

  HOOK_RESTORE
  jr ra
   nop

/*
Replaces:
  sw      zero, 0x0098 (s0)
*/
PATCH_START 0x8009b448
  jal     Play_TransitionDone_Hook
PATCH_END

/*
Replaces:
  sw      zero, 0x0098 (s0)
*/
PATCH_START 0x8009b6a0
  jal     Play_TransitionDone_Hook
PATCH_END

/*
Replaces:
  sw      zero, 0x0098 (s0)
*/
PATCH_START 0x8009b884
  jal     Play_TransitionDone_Hook
PATCH_END

/*
Replaces:
  sw      zero, 0x0098 (s0)
*/
PATCH_START 0x8009ba18
  jal     Play_TransitionDone_Hook
PATCH_END

/*
Replaces:
  sll     t0, v0, 2
  lui     t2, 0x8010
  sll     t9, t8, 2
  addu    t1, t9, t0
  addu    t2, t2, t1
  lhu     t2, 0x9c92 (t2)
  andi    t3, t2, 0x8000
*/
PATCH_START 0x8009B0d0
  jal     Entrance_GetTransitionFlags
   addu   a0, t8, v0
  andi    t3, v0, 0x8000
  nop
  nop
  nop
  nop
PATCH_END
