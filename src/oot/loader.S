#include <combo/asm.h>

#ifdef GAME_OOT
#define CART_ADDR     0x03fc0000
#define RAM_ADDR      0x80400000
#define LOADER_ADDR   0x80006600
#endif

#ifdef GAME_MM
#define CART_ADDR     0x03fe0000
#define RAM_ADDR      0x80000000
#define LOADER_ADDR   0x80000000
#endif

#define PAYLOAD_SIZE  0x00020000

PATCH_START(LOADER_ADDR)
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

  /* Return */
  jr   ra
  nop
PATCH_END
