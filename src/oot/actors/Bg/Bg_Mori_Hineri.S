#include <combo.h>

/* Use our wrapper */
PATCH_START 0x80913528
  la t0,BgMoriHineri_DrawWrapper
PATCH_END

PATCH_START 0x809137d0
  lui t6,%HI(BgMoriHineri_DrawWrapper)
PATCH_END

PATCH_START 0x809137d8
  addiu t6,%LO(BgMoriHineri_DrawWrapper)
PATCH_END

/* Remove relocs */

PATCH_START 0x80913dbc
.int 0
.int 0
PATCH_END

PATCH_START 0x80913e04
.int 0
.int 0
PATCH_END
