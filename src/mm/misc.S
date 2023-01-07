#include <combo.h>

/* Disable filters on pictobox - for emu */
PATCH_START 0x80165e28
  nop
PATCH_END

/* Disable all entrance cutscenes */
PATCH_START 0x800edcc4
  li t7,1
PATCH_END
