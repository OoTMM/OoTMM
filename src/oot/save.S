#include <combo.h>

/* Replace SaveGame */
PATCH_START 0x800905d4
  j comboWriteSave
   nop
PATCH_END

/* Hook after save load */
PATCH_START 0x800905cc
  j Sram_AfterOpenSave
PATCH_END

/* Remove failsafe for zelda letter */
PATCH_START 0x80090480
  li v0,0
PATCH_END

/* Remove failsafe for spoiling items */
PATCH_START 0x80090560
  li t7,0xfff
PATCH_END
