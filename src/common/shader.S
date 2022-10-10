#include <combo.h>

#if defined(GAME_OOT)

/* Patch the Piece of Heart / Heart Container object */
PATCH_OBJECT 0x00bd, 0x1290
.quad 0x0
PATCH_END

PATCH_OBJECT 0x00bd, 0x12b0
.quad 0x0
PATCH_END

PATCH_OBJECT 0x00bd, 0x1470
.quad 0x0
PATCH_END

PATCH_OBJECT 0x00bd, 0x1490
.quad 0x0
PATCH_END

PATCH_OBJECT 0x00bd, 0x1590
.quad 0x0
PATCH_END

PATCH_OBJECT 0x00bd, 0x15b0
.quad 0x0
PATCH_END

PATCH_OBJECT 0x00bd, 0x16d8
.quad 0x0
PATCH_END

PATCH_OBJECT 0x00bd, 0x16e0
.quad 0x0
PATCH_END

#endif
