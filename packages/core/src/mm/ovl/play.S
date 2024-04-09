#include <combo.h>

/* Hook into Play_Init */
PATCH_START 0x801bd9b8
.int hookPlay_Init
PATCH_END

/* Wrap Play_Update */
PATCH_START 0x8016a8a8
  lui t4,%HI(Play_UpdateWrapper)
PATCH_END

PATCH_START 0x8016a8b0
  addiu t4,%LO(Play_UpdateWrapper)
PATCH_END

/* Transition hooks */
Play_TransitionDone_Hook:
  sb      zero, 0x009b (a2)
  HOOK_SAVE
  jal     Play_TransitionDone
   or     a0, s0, zero

  HOOK_RESTORE
  jr ra
   nop

/*
Replaces:
  sb      zero, 0x009b (a2)
*/
PATCH_START 0x80167030
  jal     Play_TransitionDone_Hook
PATCH_END

/*
Replaces:
  sb      zero, 0x009b (a2)
*/
PATCH_START 0x801672d0
  jal     Play_TransitionDone_Hook
PATCH_END

/*
Replaces:
  sb      zero, 0x009b (a2)
*/
PATCH_START 0x801674c8
  jal     Play_TransitionDone_Hook
PATCH_END

/*
Replaces:
  sb      zero, 0x009b (a2)
*/
PATCH_START 0x80167650
  jal     Play_TransitionDone_Hook
PATCH_END

/* Disable special casing of grotto exit */
PATCH_START 0x80835500
  nop
PATCH_END

PATCH_START 0x80835508
  nop
PATCH_END
