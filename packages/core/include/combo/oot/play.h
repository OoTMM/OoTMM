#ifndef OOT_PLAY_H
#define OOT_PLAY_H

#include <combo/util.h>
#include <combo/game_state.h>
#include <combo/common/actor.h>
#include <combo/oot/regs.h>
#include <combo/common/color.h>

#define AGE_ADULT 0
#define AGE_CHILD 1

typedef struct Actor_Player Actor_Player;

/* Start: 0x1d64 */
typedef struct PACKED
{
    char unk_00[4];
    u32  next;
    u8   active;
    char unk_09[0x1f];
    u16* unk_28;
    char unk_2c[0x4];
}
CutsceneContext;

typedef struct PACKED
{
    char view[0x128];
    char unk_0128[0xdc88];
    char textBuffer[0x550]; /* Probably smaller */
    /* 0xE300 */ s32 msgLength;
    /* 0xE304 */ u8 msgMode;
    /* 0xE305 */ u8 unk_e305;
    char unk_e306[0xdf];
    u8   choice;
    char unk_e3e6[0x06];
    u16  lastSongPlayed;
    u16  ocarinaMode;
    u16  ocarinaAction;
    char unk_e3f2[0x26];
}
MessageContext;

#define OCARINA_SONG_MINUET             0x0
#define OCARINA_SONG_BOLERO             0x1
#define OCARINA_SONG_SERENADE           0x2
#define OCARINA_SONG_REQUIEM            0x3
#define OCARINA_SONG_NOCTURNE           0x4
#define OCARINA_SONG_PRELUDE            0x5
#define OCARINA_SONG_SARIAS             0x6
#define OCARINA_SONG_EPONAS             0x7
#define OCARINA_SONG_LULLABY            0x8
#define OCARINA_SONG_SUNS               0x9
#define OCARINA_SONG_TIME               0xa
#define OCARINA_SONG_STORMS             0xb
#define OCARINA_SONG_SCARECROW_SPAWN    0xc
#define OCARINA_SONG_MEMORY_GAME        0xd

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

typedef struct
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
    struct OcarinaStaff*    ocarinaStaff;
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
    char                    s27C[0x0038];
}
PauseContext;

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

_Static_assert(sizeof(PauseContext) == 0x2b4, "OoT PauseContext size is wrong");

typedef struct {
    /* 0x00 */ u8 numActors;
    /* 0x04 */ TransitionActorEntry* list;
} TransitionActorContext; // size = 0x8

typedef struct
{
    s8      num;
    u8      unk_01;
    u8      behaviorType2;
    u8      behaviorType1;
    s8      echo;
    u8      lensMode;
    void*   roomShape;
    void*   segment;
    char    unk_10[0x4];
}
Room; // size = 0x14

_Static_assert(sizeof(Room) == 0x14, "OoT Room size is wrong");

typedef struct
{
    Room            curRoom;
    Room            prevRoom;
    void*           bufPtrs[2];
    u8              unk_30;
    s8              status;
    void*           unk_34;
    char            dmaRequest[0x20];
    OSMesgQueue     loadQueue;
    OSMesg          loadMsg;
    s16             unk_74[2]; // context-specific data used by the current scene draw config
}
RoomContext;

_Static_assert(sizeof(RoomContext) == 0x78, "OoT Room size is wrong");

#define OBJECT_EXCHANGE_BANK_MAX 19

typedef struct {
    /* 0x00 */ s16         id;
    /* 0x04 */ void*       segment;
    /* 0x08 */ char        dmaRequest[0x20]; // DmaRequest  dmaRequest;
    /* 0x28 */ OSMesgQueue loadQueue;
    /* 0x40 */ OSMesg      loadMsg;
} ObjectStatus; // size = 0x44

typedef struct ObjectContext
{
    /* 0x0000 */ void*  spaceStart;
    /* 0x0004 */ void*  spaceEnd; // original name: "endSegment"
    /* 0x0008 */ u8     num; // number of objects in bank
    /* 0x0009 */ u8     unk_09;
    /* 0x000A */ u8     mainKeepIndex; // "gameplay_keep" index in bank
    /* 0x000B */ u8     subKeepIndex; // "gameplay_field_keep" or "gameplay_dangeon_keep" index in bank
    /* 0x000C */ ObjectStatus status[OBJECT_EXCHANGE_BANK_MAX];
}
ObjectContext; // size = 0x518

_Static_assert(sizeof(ObjectContext) == 0x518, "ObjectContext size is wrong");

typedef struct {
    /* 0x00 */ Vec3f    pos;
    /* 0x0C */ f32      unk_0C; // radius?
    /* 0x10 */ Color_RGB8 color;
} TargetContextEntry; // size = 0x14

_Static_assert(sizeof(TargetContextEntry) == 0x14, "TargetContextEntry size is wrong");

typedef struct {
    /* 0x00 */ Vec3f    naviRefPos; // possibly wrong
    /* 0x0C */ Vec3f    targetCenterPos;
    /* 0x18 */ Color_RGBAf naviInner;
    /* 0x28 */ Color_RGBAf naviOuter;
    /* 0x38 */ Actor*   arrowPointedActor;
    /* 0x3C */ Actor*   targetedActor;
    /* 0x40 */ f32      unk_40;
    /* 0x44 */ f32      unk_44;
    /* 0x48 */ s16      unk_48;
    /* 0x4A */ u8       activeCategory;
    /* 0x4B */ u8       unk_4B;
    /* 0x4C */ s8       unk_4C;
    /* 0x4D */ char     unk_4D[0x03];
    /* 0x50 */ TargetContextEntry arr_50[3];
    /* 0x8C */ Actor*   unk_8C;
    /* 0x90 */ Actor*   bgmEnemy; // The nearest enemy to player with the right flags that will trigger NA_BGM_ENEMY
    /* 0x94 */ Actor*   unk_94;
} TargetContext; // size = 0x98

_Static_assert(sizeof(TargetContext) == 0x98, "TargetContext size is wrong");

typedef struct ActorContext
{
    /* 0x000 */ char unk_000[0xc];
    /* 0x00c */ ActorList actors[12];
    /* 0x06c */ TargetContext targetCtx;
    /* 0x104 */ char unk_104[0x3c];
}
ActorContext;

ASSERT_OFFSET(ActorContext, actors, 0x00c);
ASSERT_OFFSET(ActorContext, targetCtx, 0x06c);
ASSERT_OFFSET(ActorContext, unk_104, 0x104);
_Static_assert(sizeof(ActorContext) == 0x140, "OOT ActorContext size is wrong");

#define TRANS_TYPE_NONE     0x00
#define TRANS_TYPE_NORMAL   0x14

#define TRANS_GFX_TRIFORCE          0x01
#define TRANS_GFX_BLACK             0x02
#define TRANS_GFX_SANDSTORM_SLOW    0x0e
#define TRANS_GFX_SANDSTORM_FAST    0x0f
#define TRANS_GFX_SHORTCUT          0x2c

typedef struct GameState_Play
{
    GameState              gs;
    u16                    sceneId;
    char                   unk_000a6[0xa];
    void*                  sceneSegment;
    char                   unk_000b4[0x1b6e];
    ActorContext           actorCtx;
    CutsceneContext        cutscene;
    char                   unk_1d94[0x344];
    MessageContext         msgCtx;
    InterfaceContext       interfaceCtx;
    PauseContext           pauseCtx;
    char                   unk_10a14[0xd90];
    ObjectContext          objectCtx;
    RoomContext            roomCtx;
    TransitionActorContext transiActorCtx;
    /* 0x11D3C */ void (*playerInit)(Actor_Player* player, struct GameState_Play* play, FlexSkeletonHeader* skelHeader);
    /* 0x11D40 */ void (*playerUpdate)(Actor_Player* player, struct GameState_Play* play, Input* input);
    /* 0x11D44 */ int (*isPlayerDroppingFish)(struct GameState_Play* play);
    /* 0x11D48 */ s32 (*startPlayerFishing)(struct GameState_Play* play);
    /* 0x11D4C */ s32 (*grabPlayer)(struct GameState_Play* play, Actor_Player* player);
    /* 0x11D50 */ s32 (*tryPlayerCsAction)(struct GameState_Play* play, Actor* actor, s32 csAction);
    /* 0x11D54 */ void (*func_11D54)(Actor_Player* player, struct GameState_Play* play);
    /* 0x11D58 */ s32 (*damagePlayer)(struct GameState_Play* play, s32 damage);
    /* 0x11D5C */ void (*talkWithPlayer)(struct GameState_Play* play, Actor* actor);
    /* 0x11D60 */ MtxF viewProjectionMtxF;
    /* 0x11DA0 */ MtxF billboardMtxF;
    /* 0x11DE0 */ Mtx* billboardMtx;
    /* 0x11DE4 */ u32 gameplayFrames;
    /* 0x11DE8 */ u8 linkAgeOnLoad;
    /* 0x11DE9 */ u8 haltAllActors;
    /* 0x11DEA */ u8 spawn;
    /* 0x11DEB */ u8 numActorEntries;
    /* 0x11DEC */ u8 numRooms;
    /* 0x11DF0 */ RomFile* roomList;
    /* 0x11DF4 */ ActorEntry* playerEntry;
    /* 0x11DF8 */ ActorEntry* actorEntryList;
    /* 0x11DFC */ void* unk_11DFC;
    /* 0x11E00 */ Spawn* spawnList;
    /* 0x11E04 */ s16* exitList;
    /* 0x11E08 */ Path* pathList;
    /* 0x11E0C */ QuestHintCmd* naviQuestHints;
    /* 0x11E10 */ void* specialEffects;
    /* 0x11E14 */ u8 skyboxId;
    /* 0x11E15 */ s8 transitionTrigger; // "fade_direction"
    /* 0x11E16 */ s16 unk_11E16;
    /* 0x11E18 */ s16 bgCoverAlpha;
    /* 0x11E1A */ s16 nextEntranceIndex;
    /* 0x11E1C */ char unk_11E1C[0x40];
    /* 0x11E5C */ s8 shootingGalleryStatus;
    /* 0x11E5D */ s8 bombchuBowlingStatus; // "bombchu_game_flag"
    /* 0x11E5E */ u8 transitionType;
    char                   unk_11e60[0x6b8];
}
GameState_Play;

ASSERT_OFFSET(GameState_Play, sceneSegment,         0x000b0);
ASSERT_OFFSET(GameState_Play, actorCtx,             0x01c24);
ASSERT_OFFSET(GameState_Play, cutscene,             0x01d64);
ASSERT_OFFSET(GameState_Play, roomCtx,              0x11cbc);
ASSERT_OFFSET(GameState_Play, transiActorCtx,       0x11d34);
ASSERT_OFFSET(GameState_Play, transitionTrigger,    0x11e15);
ASSERT_OFFSET(GameState_Play, nextEntranceIndex,    0x11e1a);

_Static_assert(sizeof(GameState_Play) == 0x12518, "OoT GameState_Play size is wrong");

typedef struct {
    /* 0x00 */ s32  regPage; // 0: no page selected (reg editor is not active); 1: first page; `REG_PAGES`: last page
    /* 0x04 */ s32  regGroup; // Indexed from 0 to `REG_GROUPS`-1. Each group has its own character to identify it.
    /* 0x08 */ s32  regCur; // Selected reg, indexed from 0 as the page start
    /* 0x0C */ s32  dPadInputPrev;
    /* 0x10 */ s32  inputRepeatTimer;
    /* 0x14 */ s16  data[REG_GROUPS * REGS_PER_GROUP]; // Accessed through *REG macros, see regs.h
} RegEditor; // size = 0x15D4

#endif
