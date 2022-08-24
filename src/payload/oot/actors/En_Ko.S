#include <combo/asm.h>

/* Disable kokiri kid at forest */
PATCH_START 0x80ad41cc
  lui t4,0xffff
PATCH_END
