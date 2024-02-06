#ifndef INCLUDED_COMBO_OOT_PAUSE_H
#define INCLUDED_COMBO_OOT_PAUSE_H

#include <combo/util.h>
#include <combo/oot/types.h>

typedef struct OcarinaStaff OcarinaStaff;

#define PAUSE_ITEM      0
#define PAUSE_MAP       1
#define PAUSE_QUEST     2
#define PAUSE_EQUIP     3
#define PAUSE_WORLD_MAP 4

typedef struct PauseContext
{
    View            view;
    u8*             iconItemSegment;
    u8*             iconItem24Segment;
    u8*             iconItemAltSegment;
    u8*             iconItemLangSegment;
    u8*             nameSegment;
    u8*             playerSegment;
    char            unk_140[0x04];
    Vtx*            itemPageVtx;
    Vtx*            equipPageVtx;
    Vtx*            mapPageVtx;
    Vtx*            questPageVtx;
    Vtx*            infoPanelVtx;
    Vtx*            itemVtx;
    Vtx*            equipVtx;
    char            unk_160[0x04];
    Vtx*            questVtx;
    Vtx*            cursorVtx;
    Vtx*            saveVtx;
    char            unk_170[0x24];
    OcarinaStaff*   ocarinaStaff;
    char            unk_198[0x20];
    OSMesgQueue     loadQueue;
    OSMesg          loadMsg;
    u16             state;
    u16             debugState;
    Vec3f           eye;
    u16             changing;
    u16             screen_prev_idx;
    u16             screen_idx;
    u16             switchPageTimer;
    u16             savePromptState;
    char            pad_1ee[0x2];
    f32             unk_1F0;
    f32             itemPageRoll;   /* rotation (-z) of the item page into the screen  */
    f32             mapPageRoll;    /* rotation (+x) of the map page into the screen   */
    f32             questPageRoll;  /* rotation (+z) of the quest page into the screen */
    f32             maskPageRoll;   /* rotation (-z) of the mask page into the screen  */
    f32             roll;
    u16             alpha;
    s16             offsetY;
    char            unk_20c[0x8];
    s16             stickAdjX;
    s16             stickAdjY;
    s16             cursorPoint[5];
    s16             cursorX[5];
    s16             cursorY[5];
    s16             dungeonMapSlot;
    s16             cursorSpecialPos;
    s16             pageSwitchInputTimer;
    u16             namedItem;
    u16             cursorItem[4];
    u16             cursorSlot[4];
    u16             equipTargetItem; /* "sl_item_no" */
    u16             equipTargetSlot; /* "sl_number" */
    u16             equipTargetCBtn;
    s16             equipAnimX;
    s16             equipAnimY;
    s16             equipAnimAlpha;
    s16             infoPanelOffsetY;
    u16             nameDisplayTimer;
    u16             nameColorSet; /* 0 = white; 1 = grey */
    s16             cursorColorSet; /* 0 = white; 4 = yellow; 8 = green */
    s16             promptChoice; /* save/continue choice: 0 = yes; 4 = no */
    s16             ocarinaSongIdx;
    u8              worldMapPoints[20]; /* 0 = hidden; 1 = displayed; 2 = highlighted */
    u8              tradeQuestLocation;
    SkelAnime       playerSkelAnime;
}
PauseContext;

ASSERT_SIZE(PauseContext, 0x2c0);
ASSERT_OFFSET(PauseContext, view,                       0x000);
ASSERT_OFFSET(PauseContext, iconItemSegment,            0x128);
ASSERT_OFFSET(PauseContext, iconItem24Segment,          0x12c);
ASSERT_OFFSET(PauseContext, iconItemAltSegment,         0x130);
ASSERT_OFFSET(PauseContext, iconItemLangSegment,        0x134);
ASSERT_OFFSET(PauseContext, nameSegment,                0x138);
ASSERT_OFFSET(PauseContext, playerSegment,              0x13c);
ASSERT_OFFSET(PauseContext, unk_140,                    0x140);
ASSERT_OFFSET(PauseContext, itemPageVtx,                0x144);
ASSERT_OFFSET(PauseContext, equipPageVtx,               0x148);
ASSERT_OFFSET(PauseContext, mapPageVtx,                 0x14c);
ASSERT_OFFSET(PauseContext, questPageVtx,               0x150);
ASSERT_OFFSET(PauseContext, infoPanelVtx,               0x154);
ASSERT_OFFSET(PauseContext, itemVtx,                    0x158);
ASSERT_OFFSET(PauseContext, equipVtx,                   0x15c);
ASSERT_OFFSET(PauseContext, unk_160,                    0x160);
ASSERT_OFFSET(PauseContext, questVtx,                   0x164);
ASSERT_OFFSET(PauseContext, cursorVtx,                  0x168);
ASSERT_OFFSET(PauseContext, saveVtx,                    0x16c);
ASSERT_OFFSET(PauseContext, unk_170,                    0x170);
ASSERT_OFFSET(PauseContext, ocarinaStaff,               0x194);
ASSERT_OFFSET(PauseContext, unk_198,                    0x198);
ASSERT_OFFSET(PauseContext, loadQueue,                  0x1b8);
ASSERT_OFFSET(PauseContext, loadMsg,                    0x1d0);
ASSERT_OFFSET(PauseContext, state,                      0x1d4);
ASSERT_OFFSET(PauseContext, debugState,                 0x1d6);
ASSERT_OFFSET(PauseContext, eye,                        0x1d8);
ASSERT_OFFSET(PauseContext, changing,                   0x1e4);
ASSERT_OFFSET(PauseContext, screen_prev_idx,            0x1e6);
ASSERT_OFFSET(PauseContext, screen_idx,                 0x1e8);
ASSERT_OFFSET(PauseContext, switchPageTimer,            0x1ea);
ASSERT_OFFSET(PauseContext, savePromptState,            0x1ec);
ASSERT_OFFSET(PauseContext, pad_1ee,                    0x1ee);
ASSERT_OFFSET(PauseContext, unk_1F0,                    0x1f0);
ASSERT_OFFSET(PauseContext, itemPageRoll,               0x1f4);
ASSERT_OFFSET(PauseContext, mapPageRoll,                0x1f8);
ASSERT_OFFSET(PauseContext, questPageRoll,              0x1fc);
ASSERT_OFFSET(PauseContext, maskPageRoll,               0x200);
ASSERT_OFFSET(PauseContext, roll,                       0x204);
ASSERT_OFFSET(PauseContext, alpha,                      0x208);
ASSERT_OFFSET(PauseContext, offsetY,                    0x20a);
ASSERT_OFFSET(PauseContext, unk_20c,                    0x20c);
ASSERT_OFFSET(PauseContext, stickAdjX,                  0x214);
ASSERT_OFFSET(PauseContext, stickAdjY,                  0x216);
ASSERT_OFFSET(PauseContext, cursorPoint,                0x218);
ASSERT_OFFSET(PauseContext, cursorX,                    0x222);
ASSERT_OFFSET(PauseContext, cursorY,                    0x22c);
ASSERT_OFFSET(PauseContext, dungeonMapSlot,             0x236);
ASSERT_OFFSET(PauseContext, cursorSpecialPos,           0x238);
ASSERT_OFFSET(PauseContext, pageSwitchInputTimer,       0x23a);
ASSERT_OFFSET(PauseContext, namedItem,                  0x23c);
ASSERT_OFFSET(PauseContext, cursorItem,                 0x23e);
ASSERT_OFFSET(PauseContext, cursorSlot,                 0x246);
ASSERT_OFFSET(PauseContext, equipTargetItem,            0x24e);
ASSERT_OFFSET(PauseContext, equipTargetSlot,            0x250);
ASSERT_OFFSET(PauseContext, equipTargetCBtn,            0x252);
ASSERT_OFFSET(PauseContext, equipAnimX,                 0x254);
ASSERT_OFFSET(PauseContext, equipAnimY,                 0x256);
ASSERT_OFFSET(PauseContext, equipAnimAlpha,             0x258);
ASSERT_OFFSET(PauseContext, infoPanelOffsetY,           0x25a);
ASSERT_OFFSET(PauseContext, nameDisplayTimer,           0x25c);
ASSERT_OFFSET(PauseContext, nameColorSet,               0x25e);
ASSERT_OFFSET(PauseContext, cursorColorSet,             0x260);
ASSERT_OFFSET(PauseContext, promptChoice,               0x262);
ASSERT_OFFSET(PauseContext, ocarinaSongIdx,             0x264);
ASSERT_OFFSET(PauseContext, worldMapPoints,             0x266);
ASSERT_OFFSET(PauseContext, tradeQuestLocation,         0x27a);
ASSERT_OFFSET(PauseContext, playerSkelAnime,            0x27c);

#endif
