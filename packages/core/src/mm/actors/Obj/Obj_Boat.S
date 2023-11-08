#include <combo.h>

/*
    File:  Pirates' Fortress Boat - Actor
    Start: 0x80b9af50 - 0x10000b0
    End:   0x80b9b6e0 - 0x1000840
    Size:  0x790
*/

ObjBoat_GetAccelSpeed_Hook:
    addiu   sp, sp, -0x18
    sw      ra, 0x0014 (sp)

    jal     ObjBoat_GetAccelSpeed
    move    a0, s0

    mfc1    a2, $f0

    lw      ra, 0x0014 (sp)
    jr      ra
    addiu   sp, sp, 0x18

ObjBoat_GetTopSpeed_Hook:
    addiu   sp, sp, -0x18
    sw      ra, 0x0014 (sp)

    jal     ObjBoat_GetTopSpeed
    move    a0, s0

    /* Move result into AT */
    mfc1    at, $f0

    /* Displaced code */
    lh      t9, 0x001C (s0)

    lw      ra, 0x0014 (sp)
    jr      ra
    addiu   sp, sp, 0x18

PATCH_START 0x80b9af50 + 0x280
    sw      a1, 0x0010 (sp)
    jal     0x800CAF70
    sw      t6, 0x0044 (sp)
    move    v1, v0
    move    a0, s0
    sw      zero, 0x003c (sp)
PATCH_END

PATCH_START 0x80b9af50 + 0x384
    bc1fl   .+0x64
PATCH_END

PATCH_START 0x80b9af50 + 0x3E8
    jal     ObjBoat_GetTopSpeed_Hook
    lw      a1, 0x0010 (sp)
PATCH_END

PATCH_START 0x80b9af50 + 0x474
    jal     ObjBoat_GetAccelSpeed_Hook
    lw      a1, 0x0010 (sp)
    lh      t5, 0x00be (s0)
PATCH_END
