#include <combo.h>

/* Use our hook to load the save */
PATCH_START 0x800902e8
  jal comboReadOwnSave
PATCH_END

/* Replace SaveGame */
PATCH_START 0x800905d4
  j PrepareAndSave
   nop
PATCH_END

/* Hook after save load */
PATCH_START 0x800902dc
  nop
PATCH_END

PATCH_START 0x800905cc
  j Sram_AfterOpenSave
PATCH_END

/* Remove failsafe for zelda letter */
PATCH_START 0x80090480
  li v0,0
PATCH_END

/* Remove failsafe for spoiling items on warp */
PATCH_START 0x80077144
  li t4,0xfff
PATCH_END

/* Remove failsafe for spoiling items on load */
PATCH_START 0x80090560
  li t7,0xfff
PATCH_END

/* Remove failsafe for spoiling items on game over? */
PATCH_START 0x800e17cc
  li t4,0xfff
PATCH_END
