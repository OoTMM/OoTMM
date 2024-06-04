#include <combo.h>

/* Always play the epona song cutscene */
PATCH_START 0x80abee54
  li t4,0
PATCH_END

/* Don't force the player into a cutscene state */
PATCH_START 0x80abee94
  nop
PATCH_END

/* Skip the whole alien cutscene */
PATCH_START 0x80abf8b4
  li t3,1
PATCH_END
