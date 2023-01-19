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
