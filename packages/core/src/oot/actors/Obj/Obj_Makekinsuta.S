#include <combo.h>

/* 
    File:   Soft Soils - Overlay
    Start:  0x80B7EBC0 - 0xEFA2E0
    End:    0x80B7ED10 - 0xEFA430
    Size:   0x150
*/

PATCH_START 0x80B7EBC0 + 0x38
    slti    at, t7, 0x0001
PATCH_END