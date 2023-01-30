#include <combo.h>

.global comboDisableInterrupts
comboDisableInterrupts:
  /* Zero some COP0 registers to disable threads */
  mfc0 a0, $12
  li a1, 0xfffffffe
  and a0, a1
  mtc0 a0, $12

  mtc0 zero, $9
  mtc0 zero, $11
  mtc0 zero, $13

  /* Disable interrupts */
  la   a1, 0xa4300000
  ori  a0, zero, 0x0555
  jr   ra
   sw  a0, 0x0c(a1)
