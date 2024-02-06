#ifndef INCLUDED_COMBO_OOT_PAUSE_H
#define INCLUDED_COMBO_OOT_PAUSE_H

#include <combo/util.h>

typedef struct OcarinaStaff OcarinaStaff;

typedef struct PauseContext
{
    char                    view[0x128]; // View view;
    u8*                     iconItemSegment;
    u8*                     iconItem24Segment;
    u8*                     iconItemAltSegment;
    u8*                     iconItemLangSegment;
    u8*                     nameSegment;
    u8*                     playerSegment;
    char                    unk_140[0x04];
    Vtx*                    itemPageVtx;
    Vtx*                    equipPageVtx;
    Vtx*                    mapPageVtx;
    Vtx*                    questPageVtx;
    Vtx*                    infoPanelVtx;
    Vtx*                    itemVtx;
    Vtx*                    equipVtx;
    char                    unk_160[0x04];
    Vtx*                    questVtx;
    Vtx*                    cursorVtx;
    Vtx*                    saveVtx;
    char                    unk_170[0x24];
    OcarinaStaff*           ocarinaStaff;
    char                    unk_198[0x20];
    OSMesgQueue             loadQueue;
    OSMesg                  loadMsg;
    u16                     state;
    u16                     debugState;
    Vec3f                   eye;
    u16                     changing;
    u16                     screen_prev_idx;
    u16                     screen_idx;
    u16                     switchPageTimer;
    u16                     savePromptState;
    char                    pad_1ee[0x2];
    f32                     unk_1F0;
    f32                     itemPageRoll; // rotation (-z) of the item page into the screen
    f32                     mapPageRoll; // rotation (+x) of the map page into the screen
    f32                     questPageRoll; // rotation (+z) of the quest page into the screen
    f32                     maskPageRoll; // rotation (-z) of the mask page into the screen
    f32                     roll;
    u16                     alpha;
    s16                     offsetY;
    char                    unk_228[0x8];
    s16                     stickAdjX;
    s16                     stickAdjY;
    s16                     item_cursor;
    s16                     map_cursor;
    s16                     quest_cursor;
    s16                     equip_cursor;
    s16                     world_map_cursor;
    s16                     item_x;
    s16                     map_x;
    s16                     quest_x;
    s16                     equipment_x;
    s16                     world_map_x;
    s16                     item_y;
    s16                     map_y;
    s16                     quest_y;
    s16                     equipment_y;
    s16                     world_map_y;
    s16                     dungeonMapSlot;
    s16                     cursor_pos;
    char                    unk_09_[0x0002];
    s16                     item_id;
    s16                     item_item;
    s16                     map_item;
    s16                     quest_item;
    s16                     equip_item;
    s16                     item_hilite;
    s16                     map_hilite;
    s16                     quest_hilite;
    s16                     equipment_hilite;
    u16                     equipTargetItem; // "sl_item_no"
    u16                     equipTargetSlot; // "sl_number"
    u16                     equipTargetCBtn;
    s16                     equipAnimX;
    s16                     equipAnimY;
    s16                     equipAnimAlpha;
    s16                     infoPanelOffsetY;
    u16                     nameDisplayTimer;
    u16                     nameColorSet; // 0 = white; 1 = grey
    s16                     cursorColorSet; // 0 = white; 4 = yellow; 8 = green
    s16                     promptChoice; // save/continue choice: 0 = yes; 4 = no
    s16                     quest_song;
    u8                      worldMapPoints[20]; // 0 = hidden; 1 = displayed; 2 = highlighted
    u8                      tradeQuestLocation;
    char                    pad_27B;
    char                    s27C[0x0044];
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
ASSERT_OFFSET(PauseContext, unk_228,                    0x20c);
ASSERT_OFFSET(PauseContext, stickAdjX,                  0x214);
ASSERT_OFFSET(PauseContext, stickAdjY,                  0x216);
ASSERT_OFFSET(PauseContext, item_cursor,                0x218);
ASSERT_OFFSET(PauseContext, map_cursor,                 0x21a);
ASSERT_OFFSET(PauseContext, quest_cursor,               0x21c);
ASSERT_OFFSET(PauseContext, equip_cursor,               0x21e);
ASSERT_OFFSET(PauseContext, world_map_cursor,           0x220);
ASSERT_OFFSET(PauseContext, item_x,                     0x222);
ASSERT_OFFSET(PauseContext, map_x,                      0x224);
ASSERT_OFFSET(PauseContext, quest_x,                    0x226);
ASSERT_OFFSET(PauseContext, equipment_x,                0x228);
ASSERT_OFFSET(PauseContext, world_map_x,                0x22a);
ASSERT_OFFSET(PauseContext, item_y,                     0x22c);
ASSERT_OFFSET(PauseContext, map_y,                      0x22e);
ASSERT_OFFSET(PauseContext, quest_y,                    0x230);
ASSERT_OFFSET(PauseContext, equipment_y,                0x232);
ASSERT_OFFSET(PauseContext, world_map_y,                0x234);
ASSERT_OFFSET(PauseContext, dungeonMapSlot,             0x236);
ASSERT_OFFSET(PauseContext, cursor_pos,                 0x238);
ASSERT_OFFSET(PauseContext, unk_09_,                    0x23a);
ASSERT_OFFSET(PauseContext, item_id,                    0x23c);
ASSERT_OFFSET(PauseContext, item_item,                  0x23e);
ASSERT_OFFSET(PauseContext, map_item,                   0x240);
ASSERT_OFFSET(PauseContext, quest_item,                 0x242);
ASSERT_OFFSET(PauseContext, equip_item,                 0x244);
ASSERT_OFFSET(PauseContext, item_hilite,                0x246);
ASSERT_OFFSET(PauseContext, map_hilite,                 0x248);
ASSERT_OFFSET(PauseContext, quest_hilite,               0x24a);
ASSERT_OFFSET(PauseContext, equipment_hilite,           0x24c);
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
ASSERT_OFFSET(PauseContext, quest_song,                 0x264);
ASSERT_OFFSET(PauseContext, worldMapPoints,             0x266);
ASSERT_OFFSET(PauseContext, tradeQuestLocation,         0x27a);
ASSERT_OFFSET(PauseContext, pad_27B,                    0x27b);

#endif
