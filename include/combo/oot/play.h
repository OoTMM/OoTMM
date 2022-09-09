#ifndef OOT_PLAY_H
#define OOT_PLAY_H

#include <combo/util.h>
#include <combo/game_state.h>

#define AGE_ADULT 0
#define AGE_CHILD 1

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
    char    unk_0ec[0x44];
}
TransitionContext;

typedef struct GameState_Play
{
    GameState           gs;
    u16                 sceneId;
    char                unk_000a6[0xfde2];
    char                textBuffer[4]; /* Real size unknown */
    char                unk_0fe8c[0x8d4];
    PauseContext        pauseCtx;
    char                unk_10a14[0xd90];
    char                objTable[4]; /* Real size unknown */
    char                unk_117a8[0x588];
    TransitionContext   transition;
    char                unk_11e60[0x6b8];
}
GameState_Play;

#define TRANS_NONE     0x00
#define TRANS_NORMAL   0x14

ASSERT_OFFSET(GameState_Play, transition, 0x11d30);
ASSERT_OFFSET(GameState_Play, transition.type, 0x11e15);
ASSERT_OFFSET(GameState_Play, transition.entrance, 0x11e1a);

_Static_assert(sizeof(TransitionContext) == 0x130, "OoT TransitionContext size is wrong");
_Static_assert(sizeof(GameState_Play) == 0x12518, "OoT GameState_Play size is wrong");

typedef struct Actor Actor;
int GiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b);

#endif
