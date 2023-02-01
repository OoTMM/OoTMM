#include <combo.h>

PATCH_START LOADER_ADDR
  addiu sp, -0x18
  sw    ra, 0x04(sp)
  sw    a0, 0x08(sp)
  sw    a1, 0x0c(sp)
  sw    a2, 0x10(sp)

  /* Load the payload */
  li    a0, (PAYLOAD_ROM | PI_DOM1_ADDR2)
  li    a1, PAYLOAD_RAM
  li    a2, PAYLOAD_SIZE
  jal   DMARomToRam
   nop

#if defined(GAME_MM)
  /* MM's DMARomToRam does not inval icache */
  li    a0,PAYLOAD_RAM
  li    a1,PAYLOAD_SIZE
  jal   osInvalICache
   nop
#endif

  /* Init */
  jal init
   nop

  /* Return */
  lw    a2,0x10(sp)
  lw    a1,0x0c(sp)
  lw    a0,0x08(sp)
  lw    ra,0x04(sp)
  addiu sp,0x18

  j LOADER_NEXT
   nop
PATCH_END
