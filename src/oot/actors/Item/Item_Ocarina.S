#include <combo.h>

/* Patch DrawGI */
PATCH_START 0x80a2b8f0
  li a1,(DRAWGI_OCARINA_TIME - 1)
PATCH_END
