#include <combo.h>

/* Patch DrawGI */
PATCH_START 0x8098cd44
  li a1,(DRAWGI_GS_TOKEN2 - 1)
PATCH_END

/* Don't destroy when picking up */
PATCH_START 0x8098CBB0
  nop
  nop
PATCH_END

/* Don't destroy when boomeranging */
PATCH_START 0x8098CC00
  nop
  nop
PATCH_END
