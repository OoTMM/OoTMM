#include <combo.h>

PATCH_GROUP PG_MM_OPEN_STT

PATCH_START 0x80bc4aa4
  jal     ActorDestroy
   lw     a0, 0x0018 (sp)
PATCH_END

PATCH_GROUP_END
