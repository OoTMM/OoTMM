#include <combo/asm.h>

#define ENTRANCE_CLOCKTOWN  0xd800
#define ENTRANCE_WOODS      0xc200
#define ENTRANCE_DKING      0x7610
#define ENTRANCE_TEMPLE1    0x3000

/* DEBUG: MM entrance */
PATCH_START 0x8014533c
  li t8,ENTRANCE_TEMPLE1
PATCH_END

/* Rebase MM's DMA Table */
PATCH_START(0x80080d14)
  lui a0, ((0x00020000 | MM_BASE) >> 16)
  lui t6, ((0x00020000 | MM_BASE) >> 16)
PATCH_END

/* Rebase MM save */
PATCH_START(0x801c67c8)
.int 0x100
.int 0x140
.int 0x180
.int 0x1c0
.int 0x200
.int 0x280
.int 0x300
.int 0x380
.int 0x400
.int 0x480
PATCH_END

/* Downsize the heap by 0x20000 */
PATCH_START(0x80174c4c)
  lui t8, 0x8076
PATCH_END

/* Hook in Main to load the payload */
PATCH_START(0x8008035c)
  jal LOADER_ADDR
PATCH_END

/* Skip the N64 logo */
PATCH_START(0x808006cc)
  li t9, 1
PATCH_END

PATCH_START(0x80800134)
  jr ra
  nop
PATCH_END

/* Skip the intro */
PATCH_START(0x8096b974)
  li v0, 0xd
PATCH_END

PATCH_START(0x8096c50c)
  nop
PATCH_END

/* Mute the title screen "START" sound */
PATCH_START(0x8096c568)
  nop
PATCH_END

/* Disable the first cycle ocarina check */
PATCH_START 0x8016a4b8
  li t2,0
PATCH_END
