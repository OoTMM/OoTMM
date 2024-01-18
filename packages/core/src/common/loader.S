#include <combo.h>

#if defined(GAME_OOT)
#define PAYLOAD_OFF 0x08
#else
#define PAYLOAD_OFF 0x10
#endif

PATCH_START LOADER_ADDR
  addiu sp,-0x20
  sw    ra,0x04(sp)
  sw    s0,0x08(sp)
  sw    s1,0x0c(sp)

  /* Load the payload metadata */
  la      a0,(PI_DOM1_ADDR2 | (COMBO_META_ROM + PAYLOAD_OFF))
  addiu   a1,sp,0x10
  jal     DMARomToRam
   li     a2,0x10
  lw      s0,0x10(sp)
  lw      s1,0x14(sp)

  /* Load the payload */
  li    a0,PI_DOM1_ADDR2
  or    a0,s0
  li    a1,PAYLOAD_RAM
  jal   DMARomToRam
   move a2,s1

#if defined(GAME_MM)
  /* MM's DMARomToRam does not inval icache */
  move  a0,s0
  move  a1,s1
  jal   osInvalICache
   nop
#endif

  /* Init */
  jal comboInit
   nop

  /* Return */
  lw    s1,0x0c(sp)
  lw    s0,0x08(sp)
  lw    ra,0x04(sp)
  addiu sp,0x20

  jr ra
   nop
PATCH_END
