#include <combo/asm.h>

PATCH_START(LOADER_ADDR)
  addiu sp, -0x14
  sw    ra, 0x0(sp)
  sw    a0, 0x4(sp)
  sw    a1, 0x8(sp)
  sw    a2, 0xc(sp)
  sw    a3, 0x10(sp)

  /* Wait for pending DMAs */
  la   a1, 0xa4600000
.Lwait1:
  lw   a0, 0x10(a1)
  andi a0, a0, 3
  bne  a0, zero, .Lwait1
  nop

  /* Prepare DMA */
  li at, (PAYLOAD_RAM & 0x0fffffff)
  sw at, 0x00(a1)
  li at, (0x10000000 | PAYLOAD_ROM)
  sw at, 0x04(a1)

  /* Start DMA */
  li at, PAYLOAD_SIZE - 1
  sw at, 0x0c(a1)

  /* Wait for DMA end */
.Lwait2:
  lw   a0, 0x10(a1)
  andi a0, a0, 3
  bne  a0, zero, .Lwait2
  nop

  /* Init */
  jal init
  nop

  /* Return */
  lw    a3,0x10(sp)
  lw    a2,0xc(sp)
  lw    a1,0x8(sp)
  lw    a0,0x4(sp)
  lw    ra,0x0(sp)
  addiu sp,0x14

  j LOADER_NEXT
  nop

PATCH_END
