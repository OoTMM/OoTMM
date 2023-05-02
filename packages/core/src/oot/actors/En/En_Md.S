#include <combo.h>

/* Change the dialogue to ask for sword and shield first */
PATCH_START 0x80ae3350
  sh      zero,0x01f8(a0)
  addiu   v1,0xa5d0
  lhu     t9,0x0ed4(v1)      /* If this is t9, it saves setting it later for the next branch. */
  li      t7,0x0090          /* Mask for "Showed Mido Sword & Shield" and "Obtained Kokiri Emerald & Deku Tree Dead" */
  and     t8,t9,t7
  bne     t8,t7,.+0x1c
    nop

  b       .+0x90
    li     v0, 0x1045          /* Mido complaining about Deku Tree's death. */

  nop
  nop
  nop
PATCH_END
