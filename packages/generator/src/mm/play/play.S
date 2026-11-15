#include <combo.h>

/* Hook into Play_Init */
PATCH_START 0x801bd9b8
.int hookPlay_Init
PATCH_END

/* Wrap Play_Main */
PATCH_START 0x8016a8a8
  lui t4,%HI(Play_MainWrapper)
PATCH_END

PATCH_START 0x8016a8b0
  addiu t4,%LO(Play_MainWrapper)
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

/* Fast transition to MM */
PATCH_START 0x80173324
  j Play_FastInit
   move a0,v0
PATCH_END

.set push
.set noreorder
PATCH_START 0x8082dfd0
  .word 0x55c0fffc
PATCH_END

PATCH_START 0x8082dfc4
  j Player_DarkLinkVoiceReturn
PATCH_END

Player_DarkLinkVoiceReturn:
  move a1,a3
  j comboMmDarkLinkControllerVoice
   addiu sp,sp,0x18

PATCH_START 0x8082ed34
  .word 0x11c00005
   addiu at,zero,1
  j Player_DarkLinkIdle
PATCH_END

Player_DarkLinkIdle:
  addiu at,zero,0xc33
  beq t6,at,1f
   addiu v0,v0,-0x1bf0
  jr ra
   nop
1:
  lui v0,0x0400
  jr ra
   ori v0,v0,0xd468
.set pop
