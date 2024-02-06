#ifndef INCLUDED_COMBO_OOT_INTERFACE_H
#define INCLUDED_COMBO_OOT_INTERFACE_H

#include <combo/util.h>

typedef struct
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
InterfaceRestrictions;

ASSERT_SIZE(InterfaceRestrictions, 0x0c);
ASSERT_OFFSET(InterfaceRestrictions, unk,           0x00);
ASSERT_OFFSET(InterfaceRestrictions, buttonB,       0x01);
ASSERT_OFFSET(InterfaceRestrictions, buttonA,       0x02);
ASSERT_OFFSET(InterfaceRestrictions, bottles,       0x03);
ASSERT_OFFSET(InterfaceRestrictions, tradeItems,    0x04);
ASSERT_OFFSET(InterfaceRestrictions, hookshot,      0x05);
ASSERT_OFFSET(InterfaceRestrictions, ocarina,       0x06);
ASSERT_OFFSET(InterfaceRestrictions, warpSongs,     0x07);
ASSERT_OFFSET(InterfaceRestrictions, sunSong,       0x08);
ASSERT_OFFSET(InterfaceRestrictions, spellWind,     0x09);
ASSERT_OFFSET(InterfaceRestrictions, spellFireLove, 0x0a);
ASSERT_OFFSET(InterfaceRestrictions, all,           0x0b);

typedef struct
{
    View                    view;
    Vtx*                    actionVtx;
    Vtx*                    beatingHeartVtx;
    u8*                     parameterSegment;
    u8*                     doActionSegment;
    u8*                     iconItemSegment;
    u8*                     mapSegment;
    u8                      mapPalette[32];
    DmaRequest              dmaRequest_160;
    DmaRequest              dmaRequest_180;
    char                    unk_1a0[0xa4];
    u16                     screenFillAlpha;
    u16                     aAlpha;         /* Also carrots alpha */
    u16                     bAlpha;         /* Also HBA score alpha */
    u16                     cLeftAlpha;
    u16                     cDownAlpha;
    u16                     cRightAlpha;
    u16                     healthAlpha;    /* also max C-Up alpha */
    u16                     magicAlpha;     /* also Rupee and Key counters alpha */
    u16                     minimapAlpha;
    s16                     startAlpha;
    char                    unk_258[0x0a];
    InterfaceRestrictions   restrictions;
    char                    unk_26e[0x002];
}
InterfaceContext;

ASSERT_SIZE(InterfaceContext, 0x270);
ASSERT_OFFSET(InterfaceContext, view,               0x000);
ASSERT_OFFSET(InterfaceContext, actionVtx,          0x128);
ASSERT_OFFSET(InterfaceContext, beatingHeartVtx,    0x12c);
ASSERT_OFFSET(InterfaceContext, parameterSegment,   0x130);
ASSERT_OFFSET(InterfaceContext, doActionSegment,    0x134);
ASSERT_OFFSET(InterfaceContext, iconItemSegment,    0x138);
ASSERT_OFFSET(InterfaceContext, mapSegment,         0x13c);
ASSERT_OFFSET(InterfaceContext, mapPalette,         0x140);
ASSERT_OFFSET(InterfaceContext, dmaRequest_160,     0x160);
ASSERT_OFFSET(InterfaceContext, dmaRequest_180,     0x180);
ASSERT_OFFSET(InterfaceContext, unk_1a0,            0x1a0);
ASSERT_OFFSET(InterfaceContext, screenFillAlpha,    0x244);
ASSERT_OFFSET(InterfaceContext, aAlpha,             0x246);
ASSERT_OFFSET(InterfaceContext, bAlpha,             0x248);
ASSERT_OFFSET(InterfaceContext, cLeftAlpha,         0x24a);
ASSERT_OFFSET(InterfaceContext, cDownAlpha,         0x24c);
ASSERT_OFFSET(InterfaceContext, cRightAlpha,        0x24e);
ASSERT_OFFSET(InterfaceContext, healthAlpha,        0x250);
ASSERT_OFFSET(InterfaceContext, magicAlpha,         0x252);
ASSERT_OFFSET(InterfaceContext, minimapAlpha,       0x254);
ASSERT_OFFSET(InterfaceContext, startAlpha,         0x256);
ASSERT_OFFSET(InterfaceContext, unk_258,            0x258);
ASSERT_OFFSET(InterfaceContext, restrictions,       0x262);
ASSERT_OFFSET(InterfaceContext, unk_26e,            0x26e);

#endif
