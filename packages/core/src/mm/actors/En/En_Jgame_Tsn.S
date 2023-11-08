#include <combo.h>

/*
    File:  Fisherman (Fisherman's Jumping Game) - Actor
    Start: 0x80c13930 - 0x10788a0
    End:   0x80c152f0 - 0x107a260
    Size:  0x19c0
*/

Fisherman_ShouldEndGame_Hook:
    j       Fisherman_ShouldEndGame
    move    a0, s0

Fisherman_ShouldPassTimerCheck_Hook:
    addiu   sp, sp, -0x18
    sw      ra, 0x0014 (sp)
    sw      v1, 0x0010 (sp)

    jal     Fisherman_ShouldPassTimerCheck
    move    a0, s0

    /* Displaced code */
    addiu   a0, s0, 0x0190

    lw      v1, 0x0010 (sp)
    lw      ra, 0x0014 (sp)
    jr      ra
    addiu   sp, sp, 0x18


PATCH_START 0x80c13930 + 0x2d8 
    lw      a3, 0x3e04 (v1)
    lw      a2, 0x3e00 (v1)
    jal     Fisherman_ShouldPassTimerCheck_Hook
    lw      a1, 0x0034 (sp)
    bnez    v0, .+0x30
    lui     a1, 0x80c1 /* Preparing to load: 0x80C15030 */
PATCH_END

PATCH_START 0x80c13930 + 0xb34
    lui     a2, 0x801f
    lw      a2, 0x3470 (a2)
    lui     a3, 0x801f
    lw      a3, 0x3474 (a3)
    jal     Fisherman_ShouldEndGame_Hook
    lw      a1, 0x002c (sp)
    beqz    v0, .+0x54
    lw      a0, 0x002c (sp)
PATCH_END
