#ifndef MM_PLAY_H
#define MM_PLAY_H

#include <combo/game_state.h>
#include <combo/common/actor.h>

typedef struct
{
    void* readBuf;
    void* saveBuf;
    char unk_00[0x20];
}
SramContext;

_Static_assert(sizeof(SramContext) == 0x28, "MM SramContext size is wrong");

typedef struct
{
    char    unk_000[0x242];
    s16     cursorIndexX[5];
    s16     cursorIndexY[5];
    char    unk_256[0x008];
    u16     cursorItem[5];
    u16     cursorSlot[5];
    char    unk_272[0x012];
    s16     cursorColorIndex;
    char    unk_286[0x04a];
}
PauseContext;

_Static_assert(sizeof(PauseContext) == 0x2d0, "MM PauseContext size is wrong");

typedef struct PACKED ALIGNED(4) GameState_Play
{
    GameState       gs;
    u16             sceneId;
    char            unk_000a6[0x01c12];
    ActorList       actors[12];
    char            unk_01d18[0x029a0];
    SramContext     sramCtx;
    char            unk_046e0[0x11c10];
    char            textBuffer[4]; /* Real size unknown */
    char            unk_162f4[0x0063e];
    u16             ocarinaMode;
    char            unk_16934[2];
    s16             ocarinaSong;
    char            unk_16938[0x003f8];
    PauseContext    pauseCtx;
    char            unk_17000[0x01875];
    u8              transitionType;
    char            unk_18876[0x00004];
    u16             nextEntrance;
    char            unk_1887c[3];
    u8              transitionGfx;
    char            unk_18880[0x2ca];
    u8              transitionMode;
    char            unk_18b4b[0x70d];
}
GameState_Play;

#define TRANS_TYPE_NONE         0x00
#define TRANS_TYPE_NORMAL       0x14

#define TRANS_GFX_BLACK     0x02

_Static_assert(sizeof(GameState_Play) == 0x19258, "MM GameState_Play size is wrong");

typedef struct Actor Actor;
int GiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b);

#endif
