#ifndef INCLUDED_COMBO_OOT_INTERFACE_H
#define INCLUDED_COMBO_OOT_INTERFACE_H

#include <combo/util.h>

typedef struct PACKED
{
    char unk_000[0x244];

    struct
    {
        u16 unk;
        u16 buttonA;
        u16 buttonB;
        u16 buttonCLeft;
        u16 buttonCDown;
        u16 buttonCRight;
        u16 health;
        u16 magic;
        u16 minimap;
        u16 start;
    }
    alpha;

    char unk_258[0x0a];

    struct
    {
        u8 unk;
        u8 buttonB;
        u8 buttonA;
        u8 bottles;
        u8 tradeItems;
        u8 hookshot;
        u8 ocarina;
        u8 warpSongs;
        u8 sunSong;
        u8 spellWind;
        u8 spellFireLove;
        u8 all;
    }
    restrictions;
    char unk_26e[0x002];
}
InterfaceContext;

ASSERT_OFFSET(InterfaceContext, unk_000,      0x000);
ASSERT_OFFSET(InterfaceContext, alpha,        0x244);
ASSERT_OFFSET(InterfaceContext, unk_258,      0x258);
ASSERT_OFFSET(InterfaceContext, restrictions, 0x262);
ASSERT_OFFSET(InterfaceContext, unk_26e,      0x26e);

#endif
