#ifndef OOT_PLAY_H
#define OOT_PLAY_H

#include <combo/util.h>
#include <combo/game_state.h>
#include <combo/common/actor.h>
#include <combo/oot/regs.h>
#include <combo/common/color.h>

#include <combo/oot/actor.h>
#include <combo/oot/cutscene.h>
#include <combo/oot/pause.h>
#include <combo/oot/message.h>
#include <combo/oot/interface.h>
#include <combo/oot/room.h>
#include <combo/oot/object.h>
#include <combo/oot/environment.h>

#define AGE_ADULT 0
#define AGE_CHILD 1

typedef struct Actor_Player Actor_Player;

#define TRANS_TYPE_NONE     0x00
#define TRANS_TYPE_NORMAL   0x14

#define TRANS_GFX_TRIFORCE          0x01
#define TRANS_GFX_BLACK             0x02
#define TRANS_GFX_SANDSTORM_SLOW    0x0e
#define TRANS_GFX_SANDSTORM_FAST    0x0f
#define TRANS_GFX_SHORTCUT          0x2c

typedef struct
{
    s32 state;
}
GameOverContext;

ASSERT_SIZE(GameOverContext, 0x4);

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
    GameOverContext        gameOverCtx;
    EnvironmentContext     envCtx;
    char                   unk_10b20[0xc84];
    ObjectContext          objectCtx;
    RoomContext            roomCtx;
    TransitionActorContext transiActorCtx;

    void (*playerInit)(Actor_Player* player, struct GameState_Play* play, FlexSkeletonHeader* skelHeader);
    void (*playerUpdate)(Actor_Player* player, struct GameState_Play* play, Input* input);
    int (*isPlayerDroppingFish)(struct GameState_Play* play);
    s32 (*startPlayerFishing)(struct GameState_Play* play);
    s32 (*grabPlayer)(struct GameState_Play* play, Actor_Player* player);
    s32 (*tryPlayerCsAction)(struct GameState_Play* play, Actor* actor, s32 csAction);
    void (*func_11D54)(Actor_Player* player, struct GameState_Play* play);
    s32 (*damagePlayer)(struct GameState_Play* play, s32 damage);
    void (*talkWithPlayer)(struct GameState_Play* play, Actor* actor);

    MtxF            viewProjectionMtxF;
    MtxF            billboardMtxF;
    Mtx*            billboardMtx;
    u32             gameplayFrames;
    u8              linkAgeOnLoad;
    u8              haltAllActors;
    u8              spawn;
    u8              numActorEntries;
    u8              numRooms;
    RomFile*        roomList;
    ActorEntry*     playerEntry;
    ActorEntry*     actorEntryList;
    void*           unk_11DFC;
    Spawn*          spawnList;
    s16*            exitList;
    Path*           pathList;
    QuestHintCmd*   naviQuestHints;
    void*           specialEffects;
    u8              skyboxId;
    s8              transitionTrigger; /* "fade_direction" */
    s16             unk_11E16;
    s16             bgCoverAlpha;
    u16             nextEntranceIndex;
    char            unk_11E1C[0x40];
    s8              shootingGalleryStatus;
    s8              bombchuBowlingStatus; /* "bombchu_game_flag" */
    u8              transitionType;
    CollisionCheckContext colChkCtx;
    char            unk_120fc[0x41c];
}
GameState_Play;

ASSERT_SIZE(GameState_Play, 0x12518);
ASSERT_OFFSET(GameState_Play, sceneSegment,             0x000b0);
ASSERT_OFFSET(GameState_Play, actorCtx,                 0x01c24);
ASSERT_OFFSET(GameState_Play, cutscene,                 0x01d64);
ASSERT_OFFSET(GameState_Play, msgCtx,                   0x020d8);
ASSERT_OFFSET(GameState_Play, interfaceCtx,             0x104f0);
ASSERT_OFFSET(GameState_Play, pauseCtx,                 0x10760);
ASSERT_OFFSET(GameState_Play, gameOverCtx,              0x10a20);
ASSERT_OFFSET(GameState_Play, envCtx,                   0x10a24);
ASSERT_OFFSET(GameState_Play, objectCtx,                0x117a4);
ASSERT_OFFSET(GameState_Play, roomCtx,                  0x11cbc);
ASSERT_OFFSET(GameState_Play, transiActorCtx,           0x11d34);
ASSERT_OFFSET(GameState_Play, transitionTrigger,        0x11e15);
ASSERT_OFFSET(GameState_Play, nextEntranceIndex,        0x11e1a);
ASSERT_OFFSET(GameState_Play, playerInit,               0x11d3c);
ASSERT_OFFSET(GameState_Play, playerUpdate,             0x11d40);
ASSERT_OFFSET(GameState_Play, isPlayerDroppingFish,     0x11d44);
ASSERT_OFFSET(GameState_Play, startPlayerFishing,       0x11d48);
ASSERT_OFFSET(GameState_Play, grabPlayer,               0x11d4c);
ASSERT_OFFSET(GameState_Play, tryPlayerCsAction,        0x11d50);
ASSERT_OFFSET(GameState_Play, func_11D54,               0x11d54);
ASSERT_OFFSET(GameState_Play, damagePlayer,             0x11d58);
ASSERT_OFFSET(GameState_Play, talkWithPlayer,           0x11d5c);
ASSERT_OFFSET(GameState_Play, viewProjectionMtxF,       0x11d60);
ASSERT_OFFSET(GameState_Play, billboardMtxF,            0x11da0);
ASSERT_OFFSET(GameState_Play, billboardMtx,             0x11de0);
ASSERT_OFFSET(GameState_Play, gameplayFrames,           0x11de4);
ASSERT_OFFSET(GameState_Play, linkAgeOnLoad,            0x11de8);
ASSERT_OFFSET(GameState_Play, haltAllActors,            0x11de9);
ASSERT_OFFSET(GameState_Play, spawn,                    0x11dea);
ASSERT_OFFSET(GameState_Play, numActorEntries,          0x11deb);
ASSERT_OFFSET(GameState_Play, numRooms,                 0x11dec);
ASSERT_OFFSET(GameState_Play, roomList,                 0x11df0);
ASSERT_OFFSET(GameState_Play, playerEntry,              0x11df4);
ASSERT_OFFSET(GameState_Play, actorEntryList,           0x11df8);
ASSERT_OFFSET(GameState_Play, unk_11DFC,                0x11dfc);
ASSERT_OFFSET(GameState_Play, spawnList,                0x11e00);
ASSERT_OFFSET(GameState_Play, exitList,                 0x11e04);
ASSERT_OFFSET(GameState_Play, pathList,                 0x11e08);
ASSERT_OFFSET(GameState_Play, naviQuestHints,           0x11e0c);
ASSERT_OFFSET(GameState_Play, specialEffects,           0x11e10);
ASSERT_OFFSET(GameState_Play, skyboxId,                 0x11e14);
ASSERT_OFFSET(GameState_Play, transitionTrigger,        0x11e15);
ASSERT_OFFSET(GameState_Play, unk_11E16,                0x11e16);
ASSERT_OFFSET(GameState_Play, bgCoverAlpha,             0x11e18);
ASSERT_OFFSET(GameState_Play, nextEntranceIndex,        0x11e1a);
ASSERT_OFFSET(GameState_Play, unk_11E1C,                0x11e1c);
ASSERT_OFFSET(GameState_Play, shootingGalleryStatus,    0x11e5c);
ASSERT_OFFSET(GameState_Play, bombchuBowlingStatus,     0x11e5d);
ASSERT_OFFSET(GameState_Play, transitionType,           0x11e5e);

typedef struct
{
    s32  regPage;       /* 0: no page selected (reg editor is not active); 1: first page; `REG_PAGES`: last page */
    s32  regGroup;      /* Indexed from 0 to `REG_GROUPS`-1. Each group has its own character to identify it. */
    s32  regCur;        /* Selected reg, indexed from 0 as the page start */
    s32  dPadInputPrev;
    s32  inputRepeatTimer;
    s16  data[REG_GROUPS * REGS_PER_GROUP]; /* Accessed through *REG macros, see regs.h */
}
RegEditor;

ASSERT_SIZE(RegEditor, 0x15d4);
ASSERT_OFFSET(RegEditor, regPage,           0x00);
ASSERT_OFFSET(RegEditor, regGroup,          0x04);
ASSERT_OFFSET(RegEditor, regCur,            0x08);
ASSERT_OFFSET(RegEditor, dPadInputPrev,     0x0c);
ASSERT_OFFSET(RegEditor, inputRepeatTimer,  0x10);
ASSERT_OFFSET(RegEditor, data,              0x14);

#endif
