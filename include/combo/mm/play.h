#ifndef MM_PLAY_H
#define MM_PLAY_H

#include <combo/game_state.h>
#include <combo/common/actor.h>

typedef struct
{
    char unk_00[0x28];
}
SramContext;

_Static_assert(sizeof(SramContext) == 0x28, "MM SramContext size is wrong");

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
    char            unk_16938[0x01f3d];
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

#define TRANS_TYPE_NONE     0x00
#define TRANS_TYPE_NORMAL   0x14

#define TRANS_GFX_BLACK     0x02

_Static_assert(sizeof(GameState_Play) == 0x19258, "MM GameState_Play size is wrong");

typedef struct Actor Actor;
int GiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b);

#endif
