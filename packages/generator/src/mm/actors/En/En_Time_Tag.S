#include <combo.h>

/* Use our wrapper */
PATCH_START 0x80ac9eb0
  lui     t6,%HI(EnTimeTag_KickOut_WaitForTime_Wrapper)
  addiu   t6,%LO(EnTimeTag_KickOut_WaitForTime_Wrapper)
PATCH_END

/* Relocs */
PATCH_START 0x80acaa24
.int 0
.int 0
PATCH_END
