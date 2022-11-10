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
