#include <combo.h>

/* Remove call to reset respawn data. It was unused anyway. */

/*
Replaces:
  jal     Play_SetRespawnData
*/
PATCH_START 0x808a3284
  nop
PATCH_END
