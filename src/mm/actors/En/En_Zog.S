#include <combo.h>

/* Spawn Mikau on the sand */
PATCH_START 0x80b938b4
  nop
PATCH_END

PATCH_START 0x80b93924
  nop
PATCH_END

/* Disable cutscenes */
PATCH_START 0x80b945b4
  nop
PATCH_END

PATCH_START 0x80b945cc
  nop
PATCH_END
