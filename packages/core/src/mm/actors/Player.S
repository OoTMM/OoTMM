#include <combo.h>

/* Expand the GI buffer */
PATCH_START 0x808420c8
  li a0,0x6000
PATCH_END

/* Force GetItemCollectBehavior return value */
PATCH_START 0x8083d554
  li v0,0xff
PATCH_END

PATCH_START 0x8083d400
  li v0,0xff
PATCH_END

PATCH_START 0x8083d428
  li v0,0xff
PATCH_END

PATCH_START 0x8083d554
  li v0,0xff
PATCH_END

PATCH_START 0x8083d2a0
  li v0,0xff
PATCH_END

/* Never decrement the razor blade health */
PATCH_START 0x80840024
  move t8,v0
PATCH_END

/* Disable first song of time cutscene */
PATCH_START 0x808528c4
  li t8,0x1c00
PATCH_END

/* Make give item treat fairies as normal items */
PATCH_START 0x80852da4
  li at,0xfff
PATCH_END

PATCH_START 0x80852dd8
  li at,0xfff
PATCH_END

/* Sun Mask DrawGiId */
PATCH_START 0x80859760
  li t8,DRAWGI_MM_MASK_SUN
PATCH_END

/* Use our Player_UpdateWrapper */
PATCH_START 0x801d0b68
.int Player_UpdateWrapper
PATCH_END

PATCH_START 0x80123100
  lui t5,%HI(Player_UpdateWrapper)
PATCH_END

PATCH_START 0x8012310c
  addiu t5,%LO(Player_UpdateWrapper)
PATCH_END

/* Ignore the owl save flag when soaring */
PATCH_START 0x80841640
  li t6,0
PATCH_END

/* Don't spawn the soaring cutscene */
PATCH_START 0x80841728
  nop
PATCH_END

PATCH_START 0x808416f4
  nop
  nop
PATCH_END

/* Always reset form on void */
PATCH_START 0x808345e8
  nop
PATCH_END

/* Write to the transition flag */
Player_WriteTransitionFlag:
  /* Replaced code */
  sh t9,-0x7786(at)

  li  t0,1
  lui t9,%HI(gIsEntranceOverride)
  sb  t0,%LO(gIsEntranceOverride)(t9)

  /* Replaced code */
  jr ra
   li t0,0x4

PATCH_START 0x808354d8
  jal Player_WriteTransitionFlag
   nop
PATCH_END

/* Reset deku form on void */
PATCH_START 0x8083bda0
  nop
  nop
  nop
  nop
  nop
PATCH_END

/* Bremen speed */
PATCH_VROM 0xCD8A10
.float 1.0
PATCH_END

/* Don't spawn the cutscene of first time transforming */
PATCH_START 0x808555E4
  nop
PATCH_END

PATCH_GROUP PG_MM_LENIENT_SPIKES

PATCH_START 0x8082da90 + 0x2a95e
.half 0x1000
PATCH_END

PATCH_START 0x8082da90 + 0x2a41e
.half 0x0000
PATCH_END

PATCH_START 0x8082da90 + 0x2a139
.byte 0x19
PATCH_END

PATCH_GROUP_END

// FD spins
PATCH_GROUP PG_MM_FD_ANYWHERE
PATCH_START 0x8082da90 + 0x5880
  nop
PATCH_END
PATCH_GROUP_END