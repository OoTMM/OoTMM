#include <combo.h>

/* Remove Owl at Swamp */
PATCH_START 0x8095a7ec
  nop
  nop
PATCH_END

/* Disable check for lens */
PATCH_START 0x8095a7c0
  li at,0xfff
PATCH_END

/* Disable auto-talk */
PATCH_START 0x8095bee4
  nop
PATCH_END
