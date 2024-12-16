#ifndef OOT_PLAY_H
#define OOT_PLAY_H

#include <combo/util.h>
#include <combo/game_state.h>
#include <combo/actor.h>
#include <combo/oot/regs.h>
#include <combo/common/color.h>
#include <combo/play/collision_context.h>

#include <combo/oot/actor.h>
#include <combo/oot/cutscene.h>
#include <combo/oot/pause.h>
#include <combo/oot/message.h>
#include <combo/oot/interface.h>
#include <combo/oot/room.h>
#include <combo/oot/object.h>
#include <combo/oot/environment.h>
#include <combo/oot/sram.h>
#include <combo/oot/skybox.h>

#define AGE_ADULT 0
#define AGE_CHILD 1

typedef struct Player Player;

#define TRANS_TRIGGER_OFF     0x00
#define TRANS_TRIGGER_START   0x14
#define TRANS_TRIGGER_END    -0x14

#define TRANS_GFX_TRIFORCE          0x01
#define TRANS_GFX_BLACK             0x02
#define TRANS_GFX_WHITE             0x03
#define TRANS_GFX_FADE_BLACK_FAST   0x04
#define TRANS_GFX_FADE_WHITE_FAST   0x05
#define TRANS_GFX_SANDSTORM_SLOW    0x0e
#define TRANS_GFX_SANDSTORM_FAST    0x0f
#define TRANS_GFX_SHORTCUT          0x2c

typedef struct
{
    u16 state;
    u8 pad02[2];
}
GameOverContext;

ASSERT_SIZE(GameOverContext, 0x4);

typedef struct PlayState
{
    GameState              state;
    u16                    sceneId;
    char                   unk_000a6[0xa];
    void*                  sceneSegment;
    View                   view;
    Camera                 mainCamera;
    Camera                 subCameras[3];
    Camera*                cameraPtrs[4];
    s16                    activeCamId;
    s16                    nextCamId;
    char                   unk_007a4[4];
    LightContext           lightCtx;
    char                   unk_007b8[0x08];
    CollisionContext       colCtx;
    ActorContext           actorCtx;
    CutsceneContext        csCtx;
    char                   unk_1db4[0x324];
    MessageContext         msgCtx;
    InterfaceContext       interfaceCtx;
    PauseContext           pauseCtx;
    GameOverContext        gameOverCtx;
    EnvironmentContext     envCtx;
    char                   unk_10b20[0xc84];
    ObjectContext          objectCtx;
    RoomContext            roomCtx;
    TransitionActorContext transiActorCtx;

    void (*playerInit)(Player* player, struct PlayState* play, FlexSkeletonHeader* skelHeader);
    void (*playerUpdate)(Player* player, struct PlayState* play, Input* input);
    int (*isPlayerDroppingFish)(struct PlayState* play);
    s32 (*startPlayerFishing)(struct PlayState* play);
    s32 (*grabPlayer)(struct PlayState* play, Player* player);
    s32 (*tryPlayerCsAction)(struct PlayState* play, Actor* actor, s32 csAction);
    void (*func_11D54)(Player* player, struct PlayState* play);
    s32 (*damagePlayer)(struct PlayState* play, s32 damage);
    void (*talkWithPlayer)(struct PlayState* play, Actor* actor);

    MtxF                    viewProjectionMtxF;
    MtxF                    billboardMtxF;
    Mtx*                    billboardMtx;
    u32                     gameplayFrames;
    u8                      linkAgeOnLoad;
    u8                      haltAllActors;
    u8                      spawn;
    u8                      numActorEntries;
    u8                      numRooms;
    RomFile*                roomList;
    ActorEntry*             playerEntry;
    ActorEntry*             actorEntryList;
    void*                   unk_11DFC;
    Spawn*                  spawnList;
    s16*                    exitList;
    Path*                   pathList;
    QuestHintCmd*           naviQuestHints;
    void*                   specialEffects;
    u8                      skyboxId;
    s8                      transitionTrigger; /* "fade_direction" */
    s16                     unk_11E16;
    s16                     bgCoverAlpha;
    u16                     nextEntranceIndex;
    char                    unk_11E1C[0x40];
    s8                      shootingGalleryStatus;
    s8                      bombchuBowlingStatus; /* "bombchu_game_flag" */
    u8                      transitionType;
    CollisionCheckContext   colChkCtx;
    char                    unk_120fc[0x41c];
}
PlayState;

ASSERT_SIZE(PlayState, 0x12518);
ASSERT_OFFSET(PlayState, sceneSegment,             0x000b0);
ASSERT_OFFSET(PlayState, view,                     0x000b8);
ASSERT_OFFSET(PlayState, colCtx,                   0x007c0);
ASSERT_OFFSET(PlayState, actorCtx,                 0x01c24);
ASSERT_OFFSET(PlayState, csCtx,                    0x01d64);
ASSERT_OFFSET(PlayState, msgCtx,                   0x020d8);
ASSERT_OFFSET(PlayState, interfaceCtx,             0x104f0);
ASSERT_OFFSET(PlayState, pauseCtx,                 0x10760);
ASSERT_OFFSET(PlayState, gameOverCtx,              0x10a20);
ASSERT_OFFSET(PlayState, envCtx,                   0x10a24);
ASSERT_OFFSET(PlayState, objectCtx,                0x117a4);
ASSERT_OFFSET(PlayState, roomCtx,                  0x11cbc);
ASSERT_OFFSET(PlayState, transiActorCtx,           0x11d34);
ASSERT_OFFSET(PlayState, transitionTrigger,        0x11e15);
ASSERT_OFFSET(PlayState, nextEntranceIndex,        0x11e1a);
ASSERT_OFFSET(PlayState, playerInit,               0x11d3c);
ASSERT_OFFSET(PlayState, playerUpdate,             0x11d40);
ASSERT_OFFSET(PlayState, isPlayerDroppingFish,     0x11d44);
ASSERT_OFFSET(PlayState, startPlayerFishing,       0x11d48);
ASSERT_OFFSET(PlayState, grabPlayer,               0x11d4c);
ASSERT_OFFSET(PlayState, tryPlayerCsAction,        0x11d50);
ASSERT_OFFSET(PlayState, func_11D54,               0x11d54);
ASSERT_OFFSET(PlayState, damagePlayer,             0x11d58);
ASSERT_OFFSET(PlayState, talkWithPlayer,           0x11d5c);
ASSERT_OFFSET(PlayState, viewProjectionMtxF,       0x11d60);
ASSERT_OFFSET(PlayState, billboardMtxF,            0x11da0);
ASSERT_OFFSET(PlayState, billboardMtx,             0x11de0);
ASSERT_OFFSET(PlayState, gameplayFrames,           0x11de4);
ASSERT_OFFSET(PlayState, linkAgeOnLoad,            0x11de8);
ASSERT_OFFSET(PlayState, haltAllActors,            0x11de9);
ASSERT_OFFSET(PlayState, spawn,                    0x11dea);
ASSERT_OFFSET(PlayState, numActorEntries,          0x11deb);
ASSERT_OFFSET(PlayState, numRooms,                 0x11dec);
ASSERT_OFFSET(PlayState, roomList,                 0x11df0);
ASSERT_OFFSET(PlayState, playerEntry,              0x11df4);
ASSERT_OFFSET(PlayState, actorEntryList,           0x11df8);
ASSERT_OFFSET(PlayState, unk_11DFC,                0x11dfc);
ASSERT_OFFSET(PlayState, spawnList,                0x11e00);
ASSERT_OFFSET(PlayState, exitList,                 0x11e04);
ASSERT_OFFSET(PlayState, pathList,                 0x11e08);
ASSERT_OFFSET(PlayState, naviQuestHints,           0x11e0c);
ASSERT_OFFSET(PlayState, specialEffects,           0x11e10);
ASSERT_OFFSET(PlayState, skyboxId,                 0x11e14);
ASSERT_OFFSET(PlayState, transitionTrigger,        0x11e15);
ASSERT_OFFSET(PlayState, unk_11E16,                0x11e16);
ASSERT_OFFSET(PlayState, bgCoverAlpha,             0x11e18);
ASSERT_OFFSET(PlayState, nextEntranceIndex,        0x11e1a);
ASSERT_OFFSET(PlayState, unk_11E1C,                0x11e1c);
ASSERT_OFFSET(PlayState, shootingGalleryStatus,    0x11e5c);
ASSERT_OFFSET(PlayState, bombchuBowlingStatus,     0x11e5d);
ASSERT_OFFSET(PlayState, transitionType,           0x11e5e);

typedef struct FileSelectState {
    GameState           state;
    Vtx*                windowVtx;
    u8*                 staticSegment;
    u8*                 parameterSegment;
    View                view;
    SramContext         sramCtx;
    char                unk_1E4[0x4];
    SkyboxContext       skyboxCtx;
    MessageContext      msgCtx;
    Font                font;
    EnvironmentContext  envCtx;
    char                unk_1C9E4[0x4];
    Vtx*                windowContentVtx;
    Vtx*                keyboardVtx;
    Vtx*                nameEntryVtx;
    u8                  n64ddFlag;
    u16                 deaths[3];
    u8                  fileNames[3][8];
    u16                 healthCapacities[3];
    u32                 questItems[3];
    s16                 n64ddFlags[3];
    s8                  defense[3];
    s16                 buttonIndex;
    s16                 confirmButtonIndex;
    s16                 menuMode;
    s16                 configMode;
    s16                 prevConfigMode;
    s16                 nextConfigMode;
    s16                 selectMode;
    s16                 selectedFileIndex;
    char                unk_1CA48[0x2];
    s16                 fileNamesY[3];
    s16                 actionTimer;
    s16                 buttonYOffsets[6];
    s16                 copyDestFileIndex;
    s16                 warningLabel;
    s16                 warningButtonIndex;
    s16                 titleLabel;
    s16                 nextTitleLabel;
    s16                 windowColor[3];
    s16                 titleAlpha[2];
    s16                 windowAlpha;
    s16                 fileButtonAlpha[3];
    s16                 nameBoxAlpha[3];
    s16                 nameAlpha[3];
    s16                 connectorAlpha[3];
    s16                 fileInfoAlpha[3];
    s16                 actionButtonAlpha[2];
    s16                 confirmButtonAlpha[2];
    s16                 optionButtonAlpha;
    s16                 nameEntryBoxAlpha;
    s16                 controlsAlpha;
    s16                 emptyFileTextAlpha;
    s16                 highlightColor[4];
    s16                 highlightPulseDir;
    s16                 unk_1CAAC;
    s16                 confirmButtonTexIndices[2];
    s16                 inputTimerX;
    s16                 inputTimerY;
    s16                 stickXDir;
    s16                 stickYDir;
    s16                 stickAdjX;
    s16                 stickAdjY;
    s16                 nameEntryBoxPosX;
    s16                 windowPosX;
    f32                 windowRot;
    s16                 kbdButton;
    s16                 charPage;
    s16                 charBgAlpha;
    s16                 charIndex;
    s16                 kbdX;
    s16                 kbdY;
    s16                 newFileNameCharCount;
    s16                 unk_1CAD6[5];
    void*               customFileInfoBufs[2];
    int                 game;
}
FileSelectState;

int Play_InCsMode(PlayState* this);

#endif
