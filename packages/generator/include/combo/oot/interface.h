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
    /* 0x01A0 */ char   unk_1a0[0x20];
    /* 0x01C0 */ OSMesgQueue loadQueue;
    /* 0x01D8 */ OSMesg loadMsg;
    /* 0x01DC */ Viewport viewport;
    /* 0x01EC */ s16    unk_1ec;
    /* 0x01EE */ u16    unk_1ee;
    /* 0x01F0 */ u16    unk_1f0;
    /* 0x01F4 */ f32    unk_1f4;
    /* 0x01F8 */ s16    naviCalling;
    /* 0x01FA */ s16    unk_1fa;
    /* 0x01FC */ s16    unk_1fc;
    /* 0x01FE */ s16    heartColorOscillator;
    /* 0x0200 */ s16    heartColorOscillatorDirection;
    /* 0x0202 */ s16    beatingHeartPrim[3];
    /* 0x0208 */ s16    beatingHeartEnv[3];
    /* 0x020E */ s16    heartsPrimR[2];
    /* 0x0212 */ s16    heartsPrimG[2];
    /* 0x0216 */ s16    heartsPrimB[2];
    /* 0x021A */ s16    heartsEnvR[2];
    /* 0x021E */ s16    heartsEnvG[2];
    /* 0x0222 */ s16    heartsEnvB[2];
    /* 0x0226 */ s16    unk_226; // Used only in unused functions
    /* 0x0228 */ s16    unk_228; // Used only in unused functions
    /* 0x022A */ s16    beatingHeartOscillator;
    /* 0x022C */ s16    beatingHeartOscillatorDirection;
    /* 0x022E */ s16    unk_22e;
    /* 0x0230 */ s16    lensMagicConsumptionTimer; // When lens is active, 1 unit of magic is consumed every time the timer reaches 0
    /* 0x0232 */ s16    counterDigits[4]; // used for key and rupee counters
    /* 0x023A */ u8     numHorseBoosts;
    /* 0x023C */ u16    unk_23C;
    u16                     hbaAmmo;
    u16                     unk_240;
    u16                     unk_242;
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
    s16                     unk_258;
    /* 0x025A */ s16    unk_25a;
    /* 0x025C */ s16    mapRoomNum;
    /* 0x025E */ s16    mapPaletteIndex; // "map_palete_no"
    /* 0x0260 */ u8     unk_260;
    /* 0x0261 */ u8     unk_261;
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
ASSERT_OFFSET(InterfaceContext, hbaAmmo,            0x23e);
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
