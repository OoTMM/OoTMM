#include <combo/asm.h>

#ifdef GAME_OOT
#define CART_ADDR     0x03fc0000
#define RAM_ADDR      0x80400000
#define LOADER_ADDR   0x80006600
#define NEXT          InitDmaManager
#endif

#ifdef GAME_MM
#define CART_ADDR     0x03fe0000
#define RAM_ADDR      0x80760000
#define LOADER_ADDR   0x800982b0
#define NEXT          0x80174bf0
#endif

#define PAYLOAD_SIZE  0x00020000

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
  li at, (RAM_ADDR & 0x0fffffff)
  sw at, 0x00(a1)
  li at, (0x10000000 | CART_ADDR)
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
  jal comboInit
  nop

  /* Return */
  lw    a3,0x10(sp)
  lw    a2,0xc(sp)
  lw    a1,0x8(sp)
  lw    a0,0x4(sp)
  lw    ra,0x0(sp)
  addiu sp,0x14

  j NEXT
  nop

PATCH_END
