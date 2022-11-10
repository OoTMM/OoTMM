#include <combo.h>

/* Patch DrawGI */
PATCH_START 0x80b4b418
  li a1,(DRAWGI_GS_TOKEN2 - 1)
PATCH_END
