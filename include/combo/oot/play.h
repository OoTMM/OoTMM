#ifndef OOT_PLAY_H
#define OOT_PLAY_H

#include <combo/game_state.h>

#define AGE_ADULT 0
#define AGE_CHILD 1

typedef struct PACKED ALIGNED(4)
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

typedef struct PACKED ALIGNED(4) GameState_Play
{
    GameState       gs;
    u16             sceneId;
    char            unk_000a6[0xfde2];
    char            textBuffer[4]; /* Real size unknown */
    char            unk_0fe8c[0x8d4];
    PauseContext    pauseCtx;
    char            unk_10a14[0xd90];
    char            objTable[4]; /* Real size unknown */
    char            unk_117a8[0x642];
    u8              spawnId;
    char            unk_11deb[0x72d];
}
GameState_Play;

_Static_assert(sizeof(GameState_Play) == 0x12518, "OoT GameState_Play size is wrong");

u32  GetCollectibleFlag(GameState_Play* play, int flag);
void SetCollectibleFlag(GameState_Play* play, int flag);
u32  GetTempFlag(GameState_Play* play, int flag);

int GiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b);

void LoadFile(void* dst, u32 vromAddr, u32 size);

void PreDraw1(Actor* actor, GameState_Play* play, int unk);
void PreDraw2(Actor* actor, GameState_Play* play, int unk);

#endif
