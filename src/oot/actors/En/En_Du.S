#include <combo.h>

/* Disable the cutscene itself */
PATCH_START 0x80953e80
  nop
PATCH_END

PATCH_START 0x80953e8c
  nop
PATCH_END

/* Skip dialogue */
PATCH_START 0x809544dc
  nop
PATCH_END

/* Change the flag check */
PATCH_START 0x80954424
  lui   t2,%HI(gSaveContext+0x0ef4) /* Item flags 0x20-0x2f */
  addiu t2,%LO(gSaveContext+0x0ef4)
  lh    t2,0x0(t2)             /* Load flags */
  andi  t2,0x0001              /* Check if flag 0x20 is set */
  li    t4,0x301f              /* Message ID */
  nop                          /* Padding */
  nop
  nop
  nop
PATCH_END
