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
