.set noreorder
.set nobopt
.set noat

#include <sys/regdef.h>

.global comboSwitchToMM
comboSwitchToMM:
  /* Disable interrupts */
  la   t0, 0xa4300000
  ori  a0, zero, 0x0555
  sw   a0, 0x0c(t0)

  /* Wait for pending DMAs */
  la   t0, 0xa4600000
.Lwait1:
  lw   a0, 0x10(t0)
  andi a0, a0, 3
  bne  a0, zero, .Lwait1
  nop

  /* Prepare 1MiB DMA */
  li at, 0x80000
  sw at, 0x00(t0)
  li at, 0x14001000
  sw at, 0x04(t0)

  /* Start DMA */
  li at, 0x100000 - 1
  sw at, 0x0c(t0)

  /* Wait for DMA end */
.Lwait2:
  lw   a0, 0x10(t0)
  andi a0, a0, 3
  bne  a0, zero, .Lwait2
  nop

  /* Jump to Majora */
  la ra, 0x80080000
  jr ra
  nop

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
