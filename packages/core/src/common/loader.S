#include <combo.h>

#if defined(GAME_OOT)
#define PAYLOAD_OFF 0x08
#else
#define PAYLOAD_OFF 0x10
#endif

#define PAYLOAD_META (0xb0000000 | (COMBO_META_ROM + PAYLOAD_OFF))

PATCH_START LOADER_ADDR
  addiu sp,-0x10
  sw    ra,0x04(sp)
  sw    s0,0x08(sp)
  sw    s1,0x0c(sp)

  /* Load the payload metadata */
  la    a0,PAYLOAD_META
  lw    s0,0(a0)
  lw    s1,4(a0)

  /* Load the payload */
  li    a0,PI_DOM1_ADDR2
  or    a0,s0
  li    a1,PAYLOAD_RAM
  move  a2,s1
  jal   DMARomToRam
   nop

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
  addiu sp,0x10

  jr ra
   nop
PATCH_END
