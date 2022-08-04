#include <combo/asm.h>

/* Rebase MM */
PATCH_START(0x80092960)
  lui t7, 0xb400
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
PATCH_START(0x8008036c)
  jal 0x800982b0
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

/* Force the file chooser to load the save */
PATCH_START(0x808147a8)
.int 0x80812e94
PATCH_END

PATCH_START(0x8081477c)
.int 0x80812d94
PATCH_END

/* Make the file chooser load the correct file index */
PATCH_START(0x80812da4)
  lui   t6,%hi(gComboCtx)
  addiu t6,%lo(gComboCtx)
  lw    t6,0xc(t6)
PATCH_END
