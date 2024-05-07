#include <combo.h>

BgIngate_EndArchery_Hook:
    addiu   sp, sp, -0x18
    sw      ra, 0x0014 (sp)
    sw      v1, 0x0010 (sp)

    jal     BgIngate_EndArchery
    move    a0, s0

    lw      v1, 0x0010 (sp)
    lw      ra, 0x0014 (sp)
    jr      ra
     addiu   sp, sp, 0x18

PATCH_START 0x8095403c
    jal     BgIngate_EndArchery_Hook
     lw      a1, 0x002c (sp)
    beqz    v0, .+0xac
    lw      t9, 0x0164 (s0)
PATCH_END