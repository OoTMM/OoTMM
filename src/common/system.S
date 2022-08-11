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

/* a0: dramAddr */
/* a1: cartAddr */
/* a2: size */
.global comboDma
comboDma:
  /* Stack */
  addiu sp,-0x8
  sw    s0,0x0(sp)
  sw    s1,0x4(sp)

  /* Wait for pending DMAs */
  la   s0, 0xa4600000
.Lwait1:
  lw   s1,0x10(s0)
  andi s1,3
  bne  s1,zero,.Lwait1
  nop

  /* Prepare DMA */
  li    s1,0x0fffffff
  and   a0,s1
  and   a1,s1
  li    s1,0x10000000
  or    a1,s1
  addiu a2,-1

  /* Do DMA */
  sw a0, 0x00(s0)
  sw a1, 0x04(s0)
  sw a2, 0x0c(s0)

  /* Wait for DMA end */
.Lwait2:
  lw   s1,0x10(s0)
  andi s1,3
  bne  s1,zero,.Lwait2
  nop

  /* Return */
  lw    s0,0x0(sp)
  lw    s1,0x4(sp)
  addiu sp,0x8
  jr ra
  nop
