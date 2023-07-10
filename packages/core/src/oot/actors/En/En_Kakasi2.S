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