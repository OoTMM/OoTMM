#include <combo/asm.h>

.global comboPatchSceneCtor
comboPatchSceneCtor:
  /* Load the entrance */
  la at, 0x8011a5d0
  lw at, 0x00(at)
  andi at, at, 0xfffc

  /* Compare with mask shop */
  xori at, at, 0x0530
  bne at, zero, .Lforward
  nop

  /* Go to Majora */
  j comboSwitchToMM
  nop

.Lforward:
  /* Forward the call to the real scene ctor */
  la at, 0x8009a750
  jr at
  nop

/* Patch the scene ctor */
PATCH_START(0x800f13e8)
  .long comboPatchSceneCtor
PATCH_END
