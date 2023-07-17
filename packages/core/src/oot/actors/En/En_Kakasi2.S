#include <combo.h>

/* 
    File:  Pierre the Scarecrow - Overlay
    Start: 0x80b79670 - 0xef4d90
    End:   0x80b79d90 - 0xef54b0
    Size:  0x720
*/

// No cutscne when Pierre spawns
PATCH_START 0x80b79670 + 0x29c
    nop
PATCH_END

PATCH_GROUP PG_OOT_FREE_SCARECROW

FreeScarecrow:
    lhu  t0, 0x670 (v0)         # Load Link's StateII
    andi at, t0, 0x0800         # Mask it with playing_ocarina bit
    li   t0, 0x0800
    jr   ra
    nop

PATCH_START 0x80b79670 + 0x208
    jal FreeScarecrow
     nop
PATCH_END

PATCH_GROUP_END
