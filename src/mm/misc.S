#include <combo.h>

/* Disable filters on pictobox - for emu */
PATCH_START 0x80165e28
  nop
PATCH_END

/* Disable all entrance cutscenes */
PATCH_START 0x800edcc4
  li t7,1
PATCH_END

/* Disable fast time flow when not having ocarina */
PATCH_START 0x80130154
  li t9,0
PATCH_END
