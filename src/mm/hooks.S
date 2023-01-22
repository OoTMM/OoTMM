#include <combo.h>

/* Rebase MM's DMA Table */
PATCH_START(0x80080d14)
  lui a0, ((0x00020000 | MM_BASE) >> 16)
  lui t6, ((0x00020000 | MM_BASE) >> 16)
PATCH_END

/* Downsize the heap by 0x30000 + 0x10000 malloc zone */
/* DEBUG */
PATCH_START 0x80174c4c
  lui t8,0x8074
PATCH_END

/* Not sure about this
PATCH_START 0x80165e34
  lui a0,0x8076
PATCH_END

PATCH_START 0x801789b0
  lui t9,0x8076
PATCH_END
*/

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

/* Epona B button fix */
PATCH_START 0x8011231c
  li t8,1
PATCH_END
