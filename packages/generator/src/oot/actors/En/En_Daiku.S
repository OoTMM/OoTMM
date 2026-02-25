#include <combo.h>

/* 
    File:  Carpenters - Actors
    Start: 0x80a8c8c0 - 0xe0f200
    End:   0x80a8e000 - 0xe10940
    Size:  0x1740
*/

/* Shorten the timer before they escape and skip the cutscene */
PATCH_START 0x80a8c8c0 + 0x1594
.word 0x00000001
PATCH_END

PATCH_START 0x80a8c8c0 + 0x15a4
.word 0x00000001
PATCH_END

PATCH_START 0x80a8c8c0 + 0x15b4
.word 0x00000001
PATCH_END

PATCH_START 0x80a8c8c0 + 0x15c4
.word 0x00000001
PATCH_END

PATCH_START 0x80a8c8c0 + 0xd64
  nop
PATCH_END
