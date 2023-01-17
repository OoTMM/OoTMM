#include <combo.h>

/* Use our draw wrapper */
PATCH_START 0x80acd63c
  lui t8,%HI(EnElforg_DrawWrapper)
PATCH_END

PATCH_START 0x80acd644
  addiu t8,%LO(EnElforg_DrawWrapper)
PATCH_END

PATCH_START 0x80acd738
  lui t9,%HI(EnElforg_DrawWrapper)
PATCH_END

PATCH_START 0x80acd744
  addiu t9,%LO(EnElforg_DrawWrapper)
PATCH_END

PATCH_START 0x80acda2c
.int EnElforg_DrawWrapper
PATCH_END

/* Remove relocs */
PATCH_START 0x80acdc40
.int 0
.int 0
PATCH_END

PATCH_START 0x80acdc64
.int 0
.int 0
PATCH_END

PATCH_START 0x80acdc84
.int 0
PATCH_END

#define FLAGS2 (gSaveContext + 0xf8 + 0x1c * 4 + 0x14)

/* Check for our flag to destroy the town fairy */
PATCH_START 0x80acc5c0
  lui  t0,%HI(FLAGS2)
  lw   t0,%LO(FLAGS2)(t0)
  andi t1,t0,0x10
PATCH_END

/* Do not set the event from the town fairy */
PATCH_START 0x80acd27c
  nop
PATCH_END

/* Pass the fairy actor as param 2 */
PATCH_START 0x80acd488
  jal EnElforg_GiveItemTown
   move a1,s0
PATCH_END

/* Disable the default fairy item behavior & load actor as param 2 */
PATCH_START 0x80acd4dc
  jal EnElforg_GiveItemDungeon
   move a1,s0
PATCH_END

/* Always play fanfare */
PATCH_START 0x80acd4f4
  li t0,99
PATCH_END
