#ifndef OOT_PLAY_H
#define OOT_PLAY_H

#include <combo/util.h>
#include <combo/game_state.h>
#include <combo/common/actor.h>

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
    char textBuffer[0x556]; /* Probably smaller */
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
    char    unk_0[0x128];
    void*   icon_item;
    void*   icon_item_24;
    void*   icon_item_s;
    void*   icon_item_lang;
    void*   name_texture;
    void*   unk_13c;
    char    unk_140[0x94];
    u16     state;
    char    unk_1d6[0xe];
    u16     changing;
    u16     screen_prev_idx;
    u16     screen_idx;
    char    unk_1ea[0x2e];
    s16     item_cursor;
    char    unk_21a[2];
    s16     quest_cursor;
    s16     equip_cursor;
    s16     map_cursor;
    s16     item_x;
    char    unk_05_[0x0004];
    s16     equipment_x;
    char    unk_06_[0x0002];
    s16     item_y;
    char    unk_07_[0x0004];
    s16     equipment_y;
    char    unk_08_[0x0004];
    s16     cursor_pos;
    char    unk_09_[0x0002];
    s16     item_id;
    s16     item_item;
    s16     map_item;
    s16     quest_item;
    s16     equip_item;
    char    unk_0A_[0x0004];
    s16     quest_hilite;
    char    unk_0B_[0x0018];
    s16     quest_song;
    char    unk_0C_[0x0016];
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
    char                objTable[4]; /* Real size unknown */
    char                unk_117a8[0x514];
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

#endif
