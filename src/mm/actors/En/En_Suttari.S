#include <combo.h>

/* Patch DrawGI */
PATCH_START 0x8098cd44
  li a1,(DRAWGI_MM_UNK7 - 1)
PATCH_END
