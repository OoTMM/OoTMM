#ifndef COMMON_OCARINA_H
#define COMMON_OCARINA_H

#include <combo/types.h>

typedef enum
{
    OCARINA_BTN_A = 0,
    OCARINA_BTN_C_DOWN,
    OCARINA_BTN_C_RIGHT,
    OCARINA_BTN_C_LEFT,
    OCARINA_BTN_C_UP,
    OCARINA_BTN_C_RIGHT_OR_C_LEFT,
    OCARINA_BTN_INVALID = 0xFF
}
OcarinaButtonIndex;

typedef struct
{
    u8 numButtons;
    u8 buttonIndex[8];
}
OcarinaSongButtons;

typedef struct OcarinaStaff
{
    u8 buttonIndex;
    u8 state;
    u8 pos;
}
OcarinaStaff;

#endif
