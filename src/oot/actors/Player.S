#include <combo.h>

/* Expand the GI buffer size to 0x6000 */
PATCH_START 0x80844f7c
  li a0,0x6008
PATCH_END

/* Allow fishing swordless */
playerHookCastRod:
  la    a0,gSaveContext
playerHookKeepRod:
  la    at,gSaveContext
  lbu   t6,0x13e2(at)
  lbu   v0,0x0068(at)
  li    at,0x59
  beq   v0,at,.LplayerHookRod_End
   li   at,0xfff
  li    at,0xff
.LplayerHookRod_End:
  jr  ra
   nop

PATCH_START 0x80831f64
  jal playerHookKeepRod
   nop
PATCH_END

PATCH_START 0x80831d8c
  sw    ra,0x00(sp)
  jal   playerHookCastRod
   nop
  lw    ra,0x00(sp)
PATCH_END

/* Disable calls to GetItemCollectBehavior */
PATCH_START 0x8083c6f0
  li v0,0xff
PATCH_END

PATCH_START 0x8083c830
  li v0,0xff
PATCH_END

PATCH_START 0x8083c858
  li v0,0xff
PATCH_END

PATCH_START 0x8083c944
  li v0,0xff
PATCH_END

/* Use our wrapper */
PATCH_START 0x800fc548
  .int Player_UpdateWrapper
PATCH_END

/* Use our dpad hook */
Player_DpadHook_Trampoline:
  j   Player_DpadHook
   lw a1,0x2c(sp)

PATCH_START 0x80831f3c
  jal Player_DpadHook_Trampoline
PATCH_END

Player_WriteTransitionFlagHook:
  /* Replaced code */
  sh t6,0x1e1a(at)

  /* Set the transition flag */
  li  v0,1
  lui t6,%HI(gIsEntranceOverride)
  sb  v0,%LO(gIsEntranceOverride)(t6)

  /* Replaced code */
  jr ra
   lh v0,0x1e1a(v1)

PATCH_START 0x808372ac
  jal Player_WriteTransitionFlagHook
   nop
PATCH_END
