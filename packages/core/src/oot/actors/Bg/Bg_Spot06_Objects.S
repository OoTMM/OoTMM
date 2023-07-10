#include <combo.h>

/* 
    File:  ???
    Start: 0x809cae60 - 0xd5afe0
    End:   0x809cc270 - 0xd5c3f0
    Size:  0x1410
*/

PATCH_START 0x809cae60 + 0x55c
    li t6, 0
PATCH_END

PATCH_START 0x809cae60 + 0x960
    nop
PATCH_END

PATCH_START 0x809cae60 + 0x61c
    lui a2, 0x3fe6
PATCH_END