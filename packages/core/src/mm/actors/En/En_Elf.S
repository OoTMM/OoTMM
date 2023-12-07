#include <combo.h>

/* GI renumbering */
PATCH_START 0x8088dfec
  li a2,GI_MM_SCOOP
PATCH_END
