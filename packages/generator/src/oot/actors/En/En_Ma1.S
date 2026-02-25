#include <combo.h>

/* Make malon spawn on the first visit to Hyrule Castle */
PATCH_START 0x809f0d04
  li v0,1
PATCH_END

/* No Malon in Market */
PATCH_START 0x809f0ce0
  li v0,0
PATCH_END

/* Make malon spawn depending on the weird egg flag */
PATCH_START 0x809f0cf4
  andi t3,t2,0x04
PATCH_END

PATCH_START 0x809f0d40
  andi t8,t7,0x04
PATCH_END

PATCH_START 0x809f0d80
  andi t1,t0,0x04
PATCH_END

PATCH_START 0x809f11ac
  andi t3,t2,0x04
PATCH_END

/* Disable an useless malon early return */
PATCH_START 0x809f11f0
  nop
  nop
PATCH_END

/* Malon Text: Skip the talon check */
PATCH_START 0x809f0a88
  andi      t3,v0,0x04
PATCH_END

/* Malon Text: epona song fix */
PATCH_START 0x809f0a4c
  lhu t6,(0xed4+((EV_OOT_CHK_SONG_EPONA >> 4) * 2))(v1) /* t6: event_chk_inf */
  li  t7,(1 << (EV_OOT_CHK_SONG_EPONA & 0xf))     /* t7: event mask */
  nop
PATCH_END

/* Malon Text: Skip first box */
PATCH_START 0x809f0af4
  li  v0,0x2043
PATCH_END

/* Change the Talon check into a check for hyrule castle */
PATCH_START 0x809f107c
  lw    t8,0x44(sp)                     /* t8: play */
  lui   t3,0x809f                       /* Replaced code, reloc */
  lhu   t8,0xa4(t8)                     /* t8: play->sceneId */
  xori  t9,t8,SCE_OOT_HYRULE_CASTLE     /* t9: play->sceneId != SCE_OOT_HYRULE_CASTLE */
PATCH_END

/* Change the epona song check for an equivalent event check */
PATCH_START 0x809f1094
  lhu t0,(0xed4+((EV_OOT_CHK_SONG_EPONA >> 4) * 2))(v0) /* t0: event_chk_inf */
  li  t1,(1 << (EV_OOT_CHK_SONG_EPONA & 0xf))     /* t1: event mask */
PATCH_END

/* Make malon accept the ocarina without talking to her */
PATCH_START 0x809f1364
  nop
  nop
PATCH_END

/* Remove some Malon code */
PATCH_START 0x809f1384
  nop
  nop
PATCH_END

PATCH_START 0x809f1398
  nop
PATCH_END
