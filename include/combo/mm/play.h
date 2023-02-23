#ifndef MM_PLAY_H
#define MM_PLAY_H

#include <combo/util.h>
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

typedef struct PACKED
{
    char unk_000[0x264];
    struct PACKED
    {
        s16 screenFill;
        s16 buttonA;
        s16 buttonB;
        s16 buttonCLeft;
        s16 buttonCDown;
        s16 buttonCRight;
        s16 health;
        s16 magic;
        s16 minimap;
        s16 start;
    }
    alpha;
    char unk_278[0x096];
    struct PACKED {
        u8 hGauge;
        u8 bButton;
        u8 aButton;
        u8 tradeItems;
        u8 songOfTime;
        u8 songOfDoubleTime;
        u8 invSongOfTime;
        u8 songOfSoaring;
        u8 songOfStorms;
        u8 masks;
        u8 pictoBox;
        u8 all;
    } restrictions;
    char unk_31a[0x02e];
}
InterfaceContext;

ASSERT_OFFSET(InterfaceContext, unk_000,        0x000);
ASSERT_OFFSET(InterfaceContext, alpha,          0x264);
ASSERT_OFFSET(InterfaceContext, unk_278,        0x278);
ASSERT_OFFSET(InterfaceContext, restrictions,   0x30e);
ASSERT_OFFSET(InterfaceContext, unk_31a,        0x31a);

typedef struct
{
    char    unk_000[0x204];
    u8      screenIndex;
    char    unk_205[0x3d];
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

ASSERT_OFFSET(PauseContext, unk_000,        0x000);
ASSERT_OFFSET(PauseContext, screenIndex,    0x204);
_Static_assert(sizeof(PauseContext) == 0x2d0, "MM PauseContext size is wrong");

typedef struct
{
    int unk;
}
GameOverContext;

typedef struct
{
    u16  unk_00;
    u16  sceneTimeSpeed;
    char unk_04[0xfc];
}
EnvironmentContext;

_Static_assert(sizeof(EnvironmentContext) == 0x100, "MM EnvironmentContext size is wrong");

typedef struct PACKED ALIGNED(4) GameState_Play
{
    GameState           gs;
    u16                 sceneId;
    char                unk_000a6[0x01c12];
    ActorList           actors[12];
    char                unk_01d18[0x029a0];
    SramContext         sramCtx;
    char                unk_046e0[0x11c10];
    char                textBuffer[4]; /* Real size unknown */
    char                unk_162f4[0x0063e];
    u16                 ocarinaMode;
    char                unk_16934[2];
    s16                 ocarinaSong;
    /* char             unk_16938[0x003f8]; */
    char                unk_16938[0x000b0];
    InterfaceContext    interfaceCtx;
    PauseContext        pauseCtx;
    GameOverContext     gameOverCtx;
    EnvironmentContext  envCtx;
    char                unk_17104[0x01771];
    u8                  transitionType;
    char                unk_18876[0x00004];
    u16                 nextEntrance;
    char                unk_1887c[3];
    u8                  transitionGfx;
    char                unk_18880[0x2ca];
    u8                  transitionMode;
    char                unk_18b4b[0x70d];
}
GameState_Play;

typedef struct
{
    char unk[0x32];
    u16  clockSpeed;
}
GameData;

extern GameData* gGameData;

ASSERT_OFFSET(GameState_Play, unk_16938,    0x16938);
ASSERT_OFFSET(GameState_Play, interfaceCtx, 0x169e8);
ASSERT_OFFSET(GameState_Play, envCtx,       0x17004);

#define TRANS_TYPE_NONE         0x00
#define TRANS_TYPE_NORMAL       0x14

#define TRANS_GFX_BLACK     0x02

_Static_assert(sizeof(GameState_Play) == 0x19258, "MM GameState_Play size is wrong");

typedef struct Actor Actor;
int GiveItem(Actor* actor, GameState_Play* play, s16 itemId, float a, float b);

#endif
