#include <combo/asm.h>

.global comboDisableInterrupts
comboDisableInterrupts:
  /* Zero some COP0 registers to disable threads */
  mtc0 zero, $12
  mtc0 zero, $9
  mtc0 zero, $11
  mtc0 zero, $13

  /* Disable interrupts */
  la   a1, 0xa4300000
  ori  a0, zero, 0x0555
  jr   ra
   sw  a0, 0x0c(a1)
