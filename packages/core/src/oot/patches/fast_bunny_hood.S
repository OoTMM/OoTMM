#include <combo.h>

bunny_hood:
    li      t0, 0x04
    lw      t1, gPlay
    lw      t1, 0x1C44(t1)     //Player in actor instance table
    beqz    t1, return
    nop

    lui     at, 0x3f80
    mtc1    at, $f22
    nop
    mul.s   $f12, $f22

    lbu     a3, 0x14f(t1)      //Worn Mask
    bne     t0, a3, return
    mtc1    t7, $f4            //Displaced

    la      a3, 0x80398BC0     //Function called when Link is walking or running
    lw      t0, 0x0664(t1)     //Link State function pointer
    bne     t0, a3, return     //Branch if Link is not walking or running forward
    nop

    lui     t0, 0x3fb0
    mtc1    t0, $f22           //f22 = 1.5
    nop
    mul.s   $f12, $f22         //f12 = 1.5 * horizontal velocity

return:
    nop
    mfc1    a1, $f12           //Displaced
    jr      ra
    nop

PATCH_GROUP PG_OOT_FAST_BUNNY_HOOD
PATCH_START 0x8083C054
    jal bunny_hood
    nop
PATCH_END
PATCH_GROUP_END