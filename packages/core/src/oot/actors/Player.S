#include <combo.h>

/* Expand the GI buffer size to 0x8000 */
PATCH_START 0x80844f7c
  li a0,0x8008
PATCH_END

PATCH_START 0x80844f78
  jal Player_AllocObjectBuffer
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

/* Remove reloc for some UseItem call */
PATCH_START 0x80853520
.int 0
PATCH_END

ChildHookHand:
  lw   a0,(gSaveContext+0x04)
  beqz a0,ChildHookHand_End
   nop
  lw t7,gDlistHookshotBodyFP
  sw t7,0(a2)

ChildHookHand_End:
  li         v0,0
  lw         ra,0x1c(sp)
  addiu      sp,0x20
  jr         ra
   nop

/* Use a dummy dlist as child for hook hand */
PATCH_START 0x8007ae90
  j ChildHookHand
PATCH_END

ChildHookAim:
  /* Replaced code */
  lw   a0,(gSaveContext+0x04)
  beqz a0,ChildHookAim_End
   nop
  lw t3,gDlistHookshotReticle

ChildHookAim_End:
  sw  t3,0x4(v1)
  sw  t2,0x0(v1)
  jr  ra
   nop

/* Use a dummy dlist as child for hook aim */
PATCH_START 0x8007b714
  jal ChildHookAim
   nop
PATCH_END

/* Use our handler to draw GI */
PATCH_START 0x8007b3fc
  jal comboPlayerDrawGI
PATCH_END

/* Use our handler to set the draw GI ID */
PATCH_START 0x808312e4
  j comboPlayerSetDrawGi
   nop
PATCH_END

/* Disable a check for heart container */
PATCH_START 0x80838590
  li at,0x7fff
PATCH_END

/* Disable a check for masks */
PATCH_START 0x80831e80
  li v0,0
PATCH_END

/* Use our wrapper */
PATCH_START 0x808323a4
  jal Player_ProcessItemButtonsWrapper
PATCH_END

PATCH_START 0x80853554
.int 0
PATCH_END

DrawMaskTrampoline:
  HOOK_CALL comboDrawExtendedMask
  jr ra
   nop

/* Use our mask drawing hook */
PATCH_START 0x808483c4
  jal DrawMaskTrampoline
   nop
  nop
  nop
  nop
  nop
  nop
PATCH_END

PATCH_START 0x80855668
.int 0
PATCH_END

/* Use our handler to draw the deku stick */
DrawDekuStickTrampoline:
  HOOK_CALL Player_DrawDekuStick
  jr ra
   nop

PATCH_START 0x8007b8b8
  jal DrawDekuStickTrampoline
   nop
  nop
PATCH_END

PATCH_START 0x80079848
  lw      a0,0x18(sp)
  j       Player_AfterSetEquipmentData
   addiu  sp,0x18
PATCH_END

/* Use our override handlers */
.globl Player_OverrideLimbDrawGameplayDefault
Player_OverrideLimbDrawGameplayDefault:
  addiu sp,-0x28
  j     _Player_OverrideLimbDrawGameplayDefault+8
   sw   s0,0x20(sp)

PATCH_START _Player_OverrideLimbDrawGameplayDefault
 j Player_OverrideLimbDrawGameplayDefaultWrapper
  nop
PATCH_END

.globl Player_OverrideLimbDrawPause
Player_OverrideLimbDrawPause:
  sw  a0,0x0(sp)
  j   _Player_OverrideLimbDrawPause+8
   sw a3,0xc(sp)

PATCH_START _Player_OverrideLimbDrawPause
 j Player_OverrideLimbDrawPauseWrapper
  nop
PATCH_END

.globl Player_OverrideLimbDrawGameplayFirstPerson
Player_OverrideLimbDrawGameplayFirstPerson:
  addiu sp,-0x20
  j   _Player_OverrideLimbDrawGameplayFirstPerson+8
   sw ra,0x1c(sp)

PATCH_START _Player_OverrideLimbDrawGameplayFirstPerson
 j Player_OverrideLimbDrawGameplayFirstPersonWrapper
  nop
PATCH_END

/* Use our hook to get the bow/sling string data */
GetBowSlingshotStringData:
  HOOK_CALL_RET Player_GetBowSlingshotStringData
  jr ra
   nop

PATCH_START 0x8007bbdc
  jal GetBowSlingshotStringData
   nop
  move t8,v0
  nop
  nop
PATCH_END
