#include <combo.h>

/* 
    File:  Bubble - Actor
    Start: 0x80913ea0 - 0xcb1620
    End:   0x80917b70 - 0xcb52f0
    Size:  0x3cd0
*/

/* Prevents a crash when using Dins Fire in the top-most room of Spirit Temple */
PATCH_START 0x80913ea0 + 0x2d77
.byte 0x00
PATCH_END