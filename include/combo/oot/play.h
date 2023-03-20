#ifndef OOT_PLAY_H
#define OOT_PLAY_H

#include <combo/util.h>
#include <combo/game_state.h>
#include <combo/common/actor.h>
#include <combo/oot/regs.h>

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
    char unk_e3f0[0x28];
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

typedef struct PACKED
{
    /* 0x0000 */ char view[0x128]; // View view;
    /* 0x0128 */ u8* iconItemSegment;
    /* 0x012C */ u8* iconItem24Segment;
    /* 0x0130 */ u8* iconItemAltSegment;
    /* 0x0134 */ u8* iconItemLangSegment;
    /* 0x0138 */ u8* nameSegment;
    /* 0x013C */ u8* playerSegment;
    /* 0x0140 */ char unk_140[0x04];
    /* 0x0144 */ Vtx* itemPageVtx;
    /* 0x0148 */ Vtx* equipPageVtx;
    /* 0x014C */ Vtx* mapPageVtx;
    /* 0x0150 */ Vtx* questPageVtx;
    /* 0x0154 */ Vtx* infoPanelVtx;
    /* 0x0158 */ Vtx* itemVtx;
    /* 0x015C */ Vtx* equipVtx;
    /* 0x0160 */ char unk_160[0x04];
    /* 0x0164 */ Vtx* questVtx;
    /* 0x0168 */ Vtx* cursorVtx;
    /* 0x016C */ Vtx* saveVtx;
    /* 0x0170 */ char unk_170[0x24];
    /* 0x0194 */ struct OcarinaStaff* ocarinaStaff;
    /* 0x0198 */ char unk_198[0x20];
    /* 0x01B8 */ OSMesgQueue loadQueue;
    /* 0x01D0 */ OSMesg loadMsg;
    /* 0x01D4 */ u16 state;
    /* 0x01D6 */ u16 debugState;
    /* 0x01D8 */ Vector3f eye;
    /* 0x01E4 */ u16 changing;
    /* 0x01E6 */ u16 screen_prev_idx;
    /* 0x01E8 */ u16 screen_idx;
    /* 0x01EA */ u16 switchPageTimer;
    /* 0x01EC */ u16 savePromptState;
    /* 0x01EE */ char pad_1ee[0x2];
    /* 0x01F0 */ f32 unk_1F0;
    /* 0x01F4 */ f32 itemPageRoll; // rotation (-z) of the item page into the screen
    /* 0x01F8 */ f32 mapPageRoll; // rotation (+x) of the map page into the screen
    /* 0x01FC */ f32 questPageRoll; // rotation (+z) of the quest page into the screen
    /* 0x0200 */ f32 maskPageRoll; // rotation (-z) of the mask page into the screen
    /* 0x0204 */ f32 roll;
    /* 0x0208 */ u16 alpha;
    /* 0x020A */ s16 offsetY;
    /* 0x020C */ char unk_228[0x8];
    /* 0x0214 */ s16 stickAdjX;
    /* 0x0216 */ s16 stickAdjY;
    /* 0x0218 */ s16 item_cursor;
    /* 0x021A */ s16 map_cursor;
    /* 0x021C */ s16 quest_cursor;
    /* 0x021E */ s16 equip_cursor;
    /* 0x0220 */ s16 world_map_cursor;
    /* 0x0222 */ s16 item_x;
    /* 0x0224 */ s16 map_x;
    /* 0x0226 */ s16 quest_x;
    /* 0x0228 */ s16 equipment_x;
    /* 0x022A */ s16 world_map_x;
    /* 0x022C */ s16 item_y;
    /* 0x022E */ s16 map_y;
    /* 0x0230 */ s16 quest_y;
    /* 0x0232 */ s16 equipment_y;
    /* 0x0234 */ s16 world_map_y;
    /* 0x0236 */ s16 dungeonMapSlot;
    /* 0x0238 */ s16 cursor_pos;
    /* 0x023A */ char unk_09_[0x0002];
    /* 0x023C */ s16 item_id;
    /* 0x023E */ s16 item_item;
    /* 0x0240 */ s16 map_item;
    /* 0x0242 */ s16 quest_item;
    /* 0x0244 */ s16 equip_item;
    /* 0x0246 */ s16 item_hilite;
    /* 0x0248 */ s16 map_hilite;
    /* 0x024A */ s16 quest_hilite;
    /* 0x024C */ s16 equipment_hilite;
    /* 0x024E */ u16 equipTargetItem; // "sl_item_no"
    /* 0x0250 */ u16 equipTargetSlot; // "sl_number"
    /* 0x0252 */ u16 equipTargetCBtn;
    /* 0x0254 */ s16 equipAnimX;
    /* 0x0256 */ s16 equipAnimY;
    /* 0x0258 */ s16 equipAnimAlpha;
    /* 0x025A */ s16 infoPanelOffsetY;
    /* 0x025C */ u16 nameDisplayTimer;
    /* 0x025E */ u16 nameColorSet; // 0 = white; 1 = grey
    /* 0x0260 */ s16 cursorColorSet; // 0 = white; 4 = yellow; 8 = green
    /* 0x0262 */ s16 promptChoice; // save/continue choice: 0 = yes; 4 = no
    /* 0x0264 */ s16 quest_song;
    /* 0x0266 */ u8 worldMapPoints[20]; // 0 = hidden; 1 = displayed; 2 = highlighted
    /* 0x027A */ u8 tradeQuestLocation;
    /* 0x027B */ char    pad_27B;
    char    s27C[0x0038];
}
PauseContext;

/* Start: 0x11d30 */
typedef struct PACKED
{
    char    unk_000[0xb8];
    u8      age;
    char    unk_0b9[0x01];
    u8      spawnId;
    char    unk_0bb[0x2a];
    u8      type;
    char    unk_0e6[0x04];
    u16     entrance;
    char    unk_0ec[0x42];
    u8      gfx;
    char    unk_12f[0x1];
}
TransitionContext;

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
}
RoomContext;

_Static_assert(sizeof(RoomContext) == 0x74, "OoT Room size is wrong");

#define OBJECT_EXCHANGE_BANK_MAX 19

typedef struct {
    /* 0x00 */ s16         id;
    /* 0x04 */ void*       segment;
    /* 0x08 */ char        dmaRequest[0x20]; // DmaRequest  dmaRequest;
    /* 0x28 */ OSMesgQueue loadQueue;
    /* 0x40 */ OSMesg      loadMsg;
} ObjectStatus; // size = 0x44

typedef struct {
    /* 0x0000 */ void*  spaceStart;
    /* 0x0004 */ void*  spaceEnd; // original name: "endSegment"
    /* 0x0008 */ u8     num; // number of objects in bank
    /* 0x0009 */ u8     unk_09;
    /* 0x000A */ u8     mainKeepIndex; // "gameplay_keep" index in bank
    /* 0x000B */ u8     subKeepIndex; // "gameplay_field_keep" or "gameplay_dangeon_keep" index in bank
    /* 0x000C */ ObjectStatus status[OBJECT_EXCHANGE_BANK_MAX];
} ObjectContext; // size = 0x518

_Static_assert(sizeof(ObjectContext) == 0x518, "ObjectContext size is wrong");

#define TRANS_TYPE_NONE     0x00
#define TRANS_TYPE_NORMAL   0x14

#define TRANS_GFX_TRIFORCE          0x01
#define TRANS_GFX_BLACK             0x02
#define TRANS_GFX_SANDSTORM_SLOW    0x0e
#define TRANS_GFX_SANDSTORM_FAST    0x0f
#define TRANS_GFX_SHORTCUT          0x2c

typedef struct GameState_Play
{
    GameState           gs;
    u16                 sceneId;
    char                unk_000a6[0x1b8a];
    ActorList           actors[12];
    char                unk_01c90[0x00d4];
    CutsceneContext     cutscene;
    char                unk_1d94[0x344];
    MessageContext      msgCtx;
    InterfaceContext    interfaceCtx;
    PauseContext        pauseCtx;
    char                unk_10a14[0xd90];
    /* 0x117A4 */ ObjectContext objectCtx;
    RoomContext         roomCtx;
    TransitionContext   transition;
    char                unk_11e60[0x6b8];
}
GameState_Play;

ASSERT_OFFSET(GameState_Play, cutscene,             0x1d64);
ASSERT_OFFSET(GameState_Play, roomCtx,              0x11cbc);
ASSERT_OFFSET(GameState_Play, transition,           0x11d30);
ASSERT_OFFSET(GameState_Play, transition.type,      0x11e15);
ASSERT_OFFSET(GameState_Play, transition.entrance,  0x11e1a);

_Static_assert(sizeof(TransitionContext) == 0x130, "OoT TransitionContext size is wrong");
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
