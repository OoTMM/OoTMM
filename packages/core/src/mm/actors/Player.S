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

/* Ignore some special GI values */
PATCH_START 0x80852da4
  li at,0x7fff
PATCH_END

PATCH_START 0x80852dac
  li at,0x7fff
PATCH_END

PATCH_START 0x80852db4
  li at,0x7fff
PATCH_END

PATCH_START 0x80852dd8
  li at,0x7fff
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

/* Use our handler to draw GI */
PATCH_START 0x80126964
  jal comboPlayerDrawGI
PATCH_END

/* Use our handler to set the draw GI ID */
PATCH_START 0x8082ece0
  j comboPlayerSetDrawGi
   nop
PATCH_END

/* GI renumbering */
PATCH_START 0x8085a878
  li  a2,GI_MM_PENDANT_OF_MEMORIES
PATCH_END

kItemToGi:
.short 0x00
.short 0x00
.short 0x00
.short GI_MM_SWORD_KOKIRI
.short GI_MM_SWORD_RAZOR
.short GI_MM_SWORD_GILDED
.short GI_MM_GREAT_FAIRY_SWORD
.short GI_MM_STICK
.short GI_MM_SWORD_KOKIRI
.short GI_MM_BOW
.short GI_MM_ARROW_FIRE
.short GI_MM_ARROW_ICE
.short GI_MM_ARROW_LIGHT
.short GI_MM_HOOKSHOT
.short GI_MM_BOMB
.short GI_MM_POWDER_KEG
.short GI_MM_BOMBCHU
.short 0x40 /* ??? */
.short GI_MM_NUT
.short GI_MM_PICTOGRAPH_BOX
.short GI_MM_OCARINA_OF_TIME
.short GI_MM_BOTTLE_EMPTY
.short GI_MM_FISH
.short 0x75 /* ??? */
.short 0x76 /* Ice Trap ..? */
.short 0x69 /* Zora egg ..? */
.short 0x6A /* Gold Dust ..? */
.short 0x6C /* ??? */
.short 0x6E /* Seahorse ..? */
.short 0x6B /* Mushroom ..? */
.short 0x70 /* Fake empty bottle ..? */
.short 0x5F /* ??? */
.short GI_MM_BUGS
.short 0x65 /* ??? */
.short 0x66 /* ??? */
.short GI_MM_POTION_RED
.short GI_MM_POTION_BLUE
.short GI_MM_POTION_GREEN
.short 0x61 /* Half milk ..? */
.short 0x61 /* Half milk ..? */
.short GI_MM_CHATEAU
.short GI_MM_FAIRY
.short GI_MM_MOON_TEAR
.short GI_MM_DEED_LAND
.short GI_MM_ROOM_KEY
.short GI_MM_LETTER_TO_KAFEI
.short GI_MM_MAGIC_BEAN
.short GI_MM_DEED_SWAMP
.short GI_MM_DEED_MOUNTAIN
.short GI_MM_DEED_OCEAN
.short GI_MM_MOON_TEAR
.short GI_MM_LETTER_TO_MAMA
.short 0xA7
.short 0xA8
.short GI_MM_PENDANT_OF_MEMORIES
.short GI_MM_PENDANT_OF_MEMORIES
.short GI_MM_PENDANT_OF_MEMORIES
.short GI_MM_PENDANT_OF_MEMORIES
.short GI_MM_MASK_TRUTH
.short GI_MM_MASK_KAFEI
.short GI_MM_MASK_ALL_NIGHT
.short GI_MM_MASK_BUNNY
.short GI_MM_MASK_KEATON
.short GI_MM_MASK_GARO
.short GI_MM_MASK_ROMANI
.short GI_MM_MASK_TROUPE_LEADER
.short GI_MM_MASK_POSTMAN
.short GI_MM_MASK_COUPLE
.short GI_MM_MASK_GREAT_FAIRY
.short GI_MM_MASK_GIBDO
.short GI_MM_MASK_DON_GERO
.short GI_MM_MASK_KAMARO
.short GI_MM_MASK_CAPTAIN
.short GI_MM_MASK_STONE
.short GI_MM_MASK_BREMEN
.short GI_MM_MASK_BLAST
.short GI_MM_MASK_SCENTS
.short GI_MM_MASK_GIANT
.short GI_MM_MASK_FIERCE_DEITY
.short GI_MM_MASK_GORON
.short GI_MM_MASK_ZORA
.short GI_MM_MASK_DEKU
.short GI_MM_LENS
.short 0x00

PATCH_START 0x80853c38
  nop
PATCH_END

GiFromItem:
  lui t6,%HI(kItemToGi)
  sll t5,1
  addu t6,t5
  jr ra
   lhu t6,%LO(kItemToGi)(t6)

PATCH_START 0x80853c40
  jal GiFromItem
   nop
PATCH_END

PATCH_START 0x80853ab0
  nop
PATCH_END

PATCH_START 0x80853abc
  nop
PATCH_END

GiFromItem2:
  lui t2,%HI(kItemToGi)
  sll v0,1
  addu t2,v0
  jr ra
   lhu t2,%LO(kItemToGi)(t2)

PATCH_START 0x80853b0c
  jal GiFromItem2
PATCH_END

/* Relocs */
PATCH_START 0x80861c44
.int 0
.int 0
PATCH_END

PATCH_START 0x80861c5c
.int 0
.int 0
PATCH_END
