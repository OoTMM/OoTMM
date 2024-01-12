#include <combo.h>

/* Resize */
PATCH_START 0x800e775c
.int 0x1a4
PATCH_END

/* Use custom init */
PATCH_START 0x800e7760
.int EnItem00_InitWrapper
PATCH_END

/* Use our xflag post-collection handler */
PATCH_START 0x80012f9c
  jal EnItem00_SetXflagCollectedHandler
PATCH_END

/* Use our xflag add handler */
PATCH_START 0x80012cac
  move a0,s0
  jal EnItem00_AddXflag
   nop
PATCH_END

/* Make small keys use the same draw function as heart pieces */
PATCH_START 0x80105fe0
.int 0x80013014
PATCH_END

/* DrawGI IDs */
PATCH_START 0x800130d8
  li a1,(DRAWGI_RECOVERY_HEART - 1)
PATCH_END

PATCH_START 0x80013100
  li a1,(DRAWGI_OOT_SHIELD_DEKU - 1)
PATCH_END

PATCH_START 0x80013114
  li a1,(DRAWGI_OOT_SHIELD_HYLIAN - 1)
PATCH_END

PATCH_START 0x80013128
  li a1,(DRAWGI_OOT_TUNIC_ZORA - 1)
PATCH_END

PATCH_START 0x8001313c
  li a1,(DRAWGI_OOT_TUNIC_GORON - 1)
PATCH_END

PATCH_START 0x80012174
  nop
PATCH_END

/* Draw bombchu drops */
PATCH_START 0x800e7800
.int 0x0403fd80
PATCH_END

ChuDropSelectTex:
  /* Replaced code */
  lh    v1,0x1c(t7)
  beq   v1,at,ChuDropSelectTex_SpecialBombs
   nop
  li    at,0x05
  beq   v1,at,ChuDropSelectTex_Bombchu
   nop
  j     0x800132c8
   slti at,v1,0x8

ChuDropSelectTex_SpecialBombs:
  j   0x800132d4
   li a2,1

ChuDropSelectTex_Bombchu:
  j   0x800132d4
   li a2,11

PATCH_START 0x800132b0
  j ChuDropSelectTex
   nop
PATCH_END

/* Give chus */
PATCH_START 0x800120be
.short ITEM_OOT_BOMBCHU_5
PATCH_END

PATCH_START 0x80012d6e
.short ITEM_OOT_BOMBCHU_5
PATCH_END

/* Use our sound effect handler */
PATCH_START 0x80012ee0
  jal EnItem00_PlaySoundXflag
   move a0,s0
PATCH_END
