#include "ultra64/asm.h"
#include "PR/R4300.h"

.set noreorder

.section .text

.balign 16

LEAF(__osGetSR)
    mfc0    $v0, C0_SR
    jr      $ra
     nop
END(__osGetSR)
