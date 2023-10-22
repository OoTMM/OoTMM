#include <combo.h>

/* 
    File:  code
    Start: 0x800a5ac0 -- 0xb3c000
    End:   0x801e3fa0 -- 0xc7a4e0
    Size:  0x13e4e0
*/

PATCH_START 0x800a5ac0 + 0x104df0
  nop
PATCH_END

// Debug screen on crash
PATCH_START 0x800a5ac0 + 0xe034
.word 0x00002725
PATCH_END

PATCH_START 0x800a5ac0 + 0xe08b
.byte 0x1c
PATCH_END

PATCH_START 0x800a5ac0 + 0xe0b3
.byte 0x1c
PATCH_END

PATCH_START 0x800a5ac0 + 0x19f00
.word 0x00002725
PATCH_END