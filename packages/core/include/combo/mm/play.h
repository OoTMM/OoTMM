#ifndef MM_PLAY_H
#define MM_PLAY_H

#include <combo/util.h>
#include <combo/game_state.h>
#include <combo/mm/light.h>
#include <combo/mm/environment.h>
#include <combo/actor.h>
#include <combo/mm/regs.h>
#include <combo/common/ocarina.h>
#include <combo/mm/object.h>
#include <combo/mm/message.h>
#include <combo/play/collision_context.h>

#define ACTORCTX_FLAG_0             (1 << 0)
#define ACTORCTX_FLAG_TELESCOPE_ON  (1 << 1)
#define ACTORCTX_FLAG_PICTO_BOX_ON  (1 << 2)
#define ACTORCTX_FLAG_3             (1 << 3)
#define ACTORCTX_FLAG_4             (1 << 4)
#define ACTORCTX_FLAG_5             (1 << 5)
#define ACTORCTX_FLAG_6             (1 << 6)
#define ACTORCTX_FLAG_7             (1 << 7)

#define GAMEMODE_NORMAL 0
#define GAMEMODE_TITLE_SCREEN 1
#define GAMEMODE_FILE_SELECT 2
#define GAMEMODE_END_CREDITS 3
#define GAMEMODE_OWL_SAVE 4

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
    char unk_000[0x174];
    u8* doActionSegment;
    char unk_178[0xaa];
    s16 bButtonInterfaceDoActionActive;
    char unk_224[0x40];
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

ASSERT_OFFSET(InterfaceContext, unk_000,                        0x000);
ASSERT_OFFSET(InterfaceContext, doActionSegment,                0x174);
ASSERT_OFFSET(InterfaceContext, bButtonInterfaceDoActionActive, 0x222);
ASSERT_OFFSET(InterfaceContext, alpha,                          0x264);
ASSERT_OFFSET(InterfaceContext, unk_278,                        0x278);
ASSERT_OFFSET(InterfaceContext, restrictions,                   0x30e);
ASSERT_OFFSET(InterfaceContext, unk_31a,                        0x31a);

typedef struct
{
    char    unk_000[0x190];
    /* 0x190 */ Vtx* vtxBuf;
    char    unk_194[0x58];
    u16     state;
    char    unk_1ee[0x12];
    u16     mainState;
    u16     nextPageMode;
    u16     screenIndex;
    u16     switchPageTimer;
    u16     savePromptState;
    /* 0x20C */ f32 unk20C;
    /* 0x210 */ f32 unk210;
    /* 0x214 */ f32 unk214;
    /* 0x218 */ f32 unk218;
    /* 0x21C */ f32 unk21C;
    /* 0x220 */ f32 unk220;
    /* 0x224 */ u16 itemAlpha;
    /* 0x226 */ u8 pad226[0x12];
    /* 0x238 */ u8 cells1[0xa]; /* PauseCells */
    s16     cursorIndexX[5];
    s16     cursorIndexY[5];
    char    unk_256[0x008];
    u16     cursorItem[5];
    u16     cursorSlot[5];
    u16     equipTargetItem;
    char    unk_274[0x010];
    s16     cursorColorIndex;
    char    unk_286[0x033];
    u8      itemDescriptionOn;
    char    unk_2ba[0x016];
}
PauseContext;

ASSERT_OFFSET(PauseContext, unk_000,            0x000);
ASSERT_OFFSET(PauseContext, state,              0x1ec);
ASSERT_OFFSET(PauseContext, mainState,          0x200);
ASSERT_OFFSET(PauseContext, screenIndex,        0x204);
ASSERT_OFFSET(PauseContext, savePromptState,    0x208);
ASSERT_OFFSET(PauseContext, itemDescriptionOn,  0x2b9);
_Static_assert(sizeof(PauseContext) == 0x2d0, "MM PauseContext size is wrong");

typedef struct
{
    int unk;
}
GameOverContext;

typedef struct TitleCardContext {
    /* 0x0 */ void* texture;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ u8 width;
    /* 0x9 */ u8 height;
    /* 0xA */ u8 durationTimer; // how long the title card appears for before fading
    /* 0xB */ u8 delayTimer; // how long the title card waits to appear
    /* 0xC */ s16 alpha;
    /* 0xE */ s16 intensity;
} TitleCardContext; // size = 0x10

typedef struct PlayerImpact {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 timer;
    /* 0x04 */ f32 dist;
    /* 0x08 */ Vec3f pos;
} PlayerImpact; // size = 0x14

typedef struct ActorContext
{
    /* 0x000 */ u8 freezeFlashTimer;
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u8 unk2;
    /* 0x003 */ u8 lensActive;
    /* 0x004 */ s8 lensMaskSize; /* The size of the circle when drawn the lens mask. Larger value leads to a smaller circle */
    /* 0x005 */ u8 flags;
    /* 0x006 */ u8 pad6[0x5];
    /* 0x00B */ s8 lensActorsDrawn;
    /* 0x00C */ s16 halfDaysBit; /* A single bit indicating the current half-day. It is one of HALFDAYBIT_DAYX_ macro values */
    /* 0x00E */ u8 totalLoadedActors;
    /* 0x00F */ u8 numLensActors;
    ActorList   actors[12];
    /* 0x0A0 */ Actor* lensActors[32]; /* LENS_ACTOR_MAX // Draws up to LENS_ACTOR_MAX number of invisible actors */
    char        unk_120[0xc4];
    /* 0x1E4 */ TitleCardContext titleCtx;
    /* 0x1F4 */ PlayerImpact playerImpact;
    char        unk_208[0x4c];
    Actor*      elegyStatues[4];
    char        unk_264[0x4];
    u8          unk_268;
    char        unk_269[0x1b];
}
ActorContext;

ASSERT_OFFSET(ActorContext, actors,         0x010);
ASSERT_OFFSET(ActorContext, unk_120,        0x120);
ASSERT_OFFSET(ActorContext, titleCtx,       0x1e4);
ASSERT_OFFSET(ActorContext, elegyStatues,   0x254);
ASSERT_OFFSET(ActorContext, unk_264,        0x264);
ASSERT_OFFSET(ActorContext, unk_268,        0x268);
ASSERT_OFFSET(ActorContext, unk_269,        0x269);

_Static_assert(sizeof(ActorContext) == 0x284,       "MM ActorContext size is wrong");

typedef struct Font
{
    union {
        u8      charBuf[2][128 * 120];
        u64     force_structure_alignment_charTex;
    };
    union {
        u8      iconBuf[128];
        u64     force_structure_alignment_icon;
    };
    union {
        u8      fontBuf[128 * 320];
        u64     force_structure_alignment_font;
    };
    union {
        char    schar[1280]; /* msgBuf */
        u16     wchar[640];  /* msgBufWide */
        u64     force_structure_alignment_msg;
    } textBuffer;
    u8*         messageStart;
    u8*         messageEnd;
    u8          unk_11d88; /* current Char Buffer ? */
}
Font; /* size = 0x11D90 */

_Static_assert(sizeof(Font) == 0x11d90, "MM Font size is wrong");

typedef struct MessageTableEntry
{
    u16 textId;
    u8 typePos;
    const char* segment;
}
MessageTableEntry; /* size = 0x8; */

_Static_assert(sizeof(MessageTableEntry) == 0x8, "MM MessageTableEntry size is wrong");

typedef struct {
    s8      recentNote;
    s8      storedSong;
    s8      noteIndex;
    char    pad3;
    s8      playbackRecentNote;
    u8      playbackState; /* 1 while doing playback, is reset to 0 to show the "You Played X song" text. */
    u8      playbackNoteIndex;
    char    pad7;
} SongFrameInfo;

/* Structure with some song state. Usually located at: 0x801FD43A. */
typedef struct {
    SongFrameInfo   frameInfo[2];
    u16             frameCount;
    s16             analogAngle; /* Angle of analog stick, modifies sound. */
    char            pad14[0x14];
    u8              hasPlayedNote; /* 1 if has played note since using ocarina. */
    char            pad29[0x7];
    u16             flags; /* 0x37DF if all songs. */
    u8              noteIndex3;
    char            pad33;
} SongInfo;

_Static_assert(sizeof(SongFrameInfo) == 0x8, "MM SongFrameInfo size is wrong");
_Static_assert(sizeof(SongInfo) == 0x34, "MM SongInfo size is wrong");

typedef struct MessageContext
{
    char                        view[0x00168]; /* TODO: Add actual View structure */
    Font                        font;
    char                        unk_11ef8[0x8];
    SongInfo*                   songInfo;
    u16                         currentTextId;
    char                        unk_11f06[0x2];
    u16                         unk_11f08;
    s8                          unk_11f0a;
    s8                          unk_11f0b;
    s8                          unk_11f0c;
    char                        unk_11f0d[0x3];
    s32                         msgLength;
    u16                         unk_11f14;
    u16                         unk_11f16;
    s8                          unk_11f18;
    char                        unk_11f19[0x1];
    s16                         unk_11f1a[0x3];
    char                        unk_11f20[0x2];
    u8                          msgMode;
    char                        unk_11f23;
    union {
        char                    schar[0xc8];
        u16                     wchar[0x64];
    } decodedBuffer;
    u16                         msgBufPos;
    s16                         unk_11fee;
    s16                         unk_11ff0;
    u16                         unk_11ff2;
    s16                         unk_11ff4;
    s16                         unk_11ff6;
    s16                         unk_11ff8;
    s16                         unk_11ffa;
    s16                         unk_11ffc;
    s16                         unk_11ffe[0x3];
    s16                         unk_12004;
    s16                         unk_12006;
    s16                         unk_12008;
    char                        unk_1200a[0x2];
    s16                         unk_1200c;
    s16                         unk_1200e;
    s16                         unk_12010;
    s16                         unk_12012;
    s16                         unk_12014;
    s16                         unk_12016;
    s16                         unk_12018; /* messageR */
    s16                         unk_1201a; /* messageG */
    s16                         unk_1201c; /* messageB */
    s16                         unk_1201e; /* messageA */
    u8                          unk_12020;  /* probably textboxEndType */
    u8                          choiceIndex;
    u8                          unk_12022;
    u8                          stateTimer;
    s16                         unk_12024;
    u16                         unk_12026;
    u16                         songPlayed;
    u16                         ocarinaMode;
    u16                         ocarinaAction;
    u16                         ocarinaSong;
    s16                         unk_12030;
    char                        unk_12032[0x2];
    s16                         unk_12034;
    s16                         unk_12036;
    s16                         unk_12038;
    s16                         unk_1203a;
    s16                         unk_1203c;
    s16                         unk_1203e;
    struct Actor*               unkActor;
    s16                         unk_12044;
    s16                         unk_12046;
    u8                          unk_12048; /* EnKakasi */
    char                        unk_12049[0x1];
    s16                         unk_1204a[0x5];
    s16                         unk_12054[6]; /* First, second and third digits in lottery code guess */
    char                        unk_12060[0x8];
    s16                         unk_12068;
    s16                         unk_1206a;
    s32                         unk_1206c;
    s32                         unk_12070;
    s32                         unk_12074;
    s32                         bankRupeesSelected;
    s32                         bankRupees;
    struct MessageTableEntry*   messageEntryTable;
    struct MessageTableEntry*   messageEntryTableNes;
    char                        unk_12088[0x4];
    struct MessageTableEntry*   messageTableStaff;
    s16                         unk_12090;
    s16                         unk_12092;
    s8                          unk_12094;
    char                        unk_12095[0x3];
    f32                         unk_12098; /* Text_Scale? */
    s16                         unk_1209c;
    char                        unk_1209e[0x2];
    s32                         unk_120a0;
    s16                         unk_120a4[6];
    u8                          unk_120b0;
    u8                          bombersNotebookEventQueueCount;
    u8                          bombersNotebookEventQueue[10];
    u16                         unk_120bc;
    s16                         unk_120be;
    s16                         unk_120c0;
    s16                         unk_120c2;
    s32                         unk_120c4;
    s16                         unk_120c8;
    s16                         unk_120ca;
    s16                         unk_120cc;
    s16                         unk_120ce;
    s16                         unk_120d0;
    s16                         unk_120d2;
    s16                         unk_120d4;
    s16                         unk_120d6;
    char                        unk_120d8[0x8];
}
MessageContext;

ASSERT_OFFSET(MessageContext, font,                 0x00168);
ASSERT_OFFSET(MessageContext, songInfo,             0x11f00);
ASSERT_OFFSET(MessageContext, currentTextId,        0x11f04);
ASSERT_OFFSET(MessageContext, decodedBuffer,        0x11f24);
ASSERT_OFFSET(MessageContext, msgBufPos,            0x11fec);
ASSERT_OFFSET(MessageContext, choiceIndex,          0x12021);
ASSERT_OFFSET(MessageContext, stateTimer,           0x12023);
ASSERT_OFFSET(MessageContext, ocarinaMode,          0x1202a);
ASSERT_OFFSET(MessageContext, unkActor,             0x12040);
ASSERT_OFFSET(MessageContext, bankRupeesSelected,   0x12078);
ASSERT_OFFSET(MessageContext, unk_120d8,            0x120d8);
_Static_assert(sizeof(MessageContext) == 0x120e0, "MM MessageContext size is wrong");

typedef struct Room
{
    s8 num;
    u8 unk_1;
    u8 behaviorType2;
    u8 behaviorType1;
    s8 echo;
    u8 lensMode;
    u8 enablePosLights;
    char unk_7;
    char roomShape_pointer[0x4];
    void* segment;
    char unk_10[0x4];
}
Room;
_Static_assert(sizeof(Room) == 0x14, "MM Room size is wrong");

typedef struct RoomContext
{
    Room curRoom;
    Room prefRoom;
    void* roomMemPages[0x2];
    u8 activeMemPage;
    s8 status;
    char unk_32[0x2];
    void* activeRoomVram;
    char unk_38[0x48];
}
RoomContext;
ASSERT_OFFSET(RoomContext, roomMemPages, 0x28);
ASSERT_OFFSET(RoomContext, unk_32, 0x32);
ASSERT_OFFSET(RoomContext, unk_38, 0x38);
_Static_assert(sizeof(RoomContext) == 0x80, "MM RoomContext size is wrong");


typedef struct {
    /* 0x0 */ u8   seqId;
    /* 0x1 */ u8   ambienceId;
} SequenceContext; /* size = 0x2 */

typedef struct {
    /* 0x000 */ GameState state;
    /* 0x0A4 */ void* daytelopStaticFile;
    /* 0x0A8 */ void* gameoverStaticFile;
    /* 0x0B0 */ View view;
    /* 0x218 */ char unk_218[0x28];
    /* 0x240 */ s16 transitionCountdown;
    /* 0x242 */ s16 fadeInState;
    /* 0x244 */ s16 alpha;
} DayTelopState; // size = 0x248

_Static_assert(sizeof(DayTelopState) == 0x248, "MM DayTelopState Size is wrong");

#define STOP_GAMESTATE(curState)     \
    do {                             \
        GameState* state = curState; \
                                     \
        state->running = 0;          \
    } while(0)

#define SET_NEXT_GAMESTATE(curState, nextInit, nextSize) \
    do {                                                 \
        GameState* state = curState;                     \
                                                         \
        (state)->nextGameStateInit = nextInit;                        \
        (state)->nextGameStateSize = nextSize;                        \
    } while (0)

typedef struct PlayState
{
    GameState           state;
    u16                 sceneId;
    u8                  sceneConfig;
    char                unk_A7[0x9];
    void*               sceneSegment;
    char                unk_B4[0x4];
    View                view;
    Camera              mainCamera;
    Camera              subCameras[3];
    Camera*             cameraPtrs[4];
    s16                 activeCamId;
    s16                 nextCamera;
    SequenceContext     sequenceCtx;
    LightContext        lightCtx;
    char                unk_00828[0x08];
    CollisionContext    colCtx;
    ActorContext        actorCtx;
    /* 0x01F24 */ CutsceneContext csCtx;
    char                unk_01f78[0x02740];
    SramContext         sramCtx;
    char                unk_046e0[0x228];
    MessageContext      msgCtx;
    InterfaceContext    interfaceCtx;
    PauseContext        pauseCtx;
    GameOverContext     gameOverCtx;
    EnvironmentContext  envCtx;
    char                unk_17104[0xc84];
    ObjectContext       objectCtx;
    RoomContext         roomCtx;
    char                unk_18760[0x3c];
    s16                 playerActorCsIds[10];
    MtxF                viewProjectionMtxF;
    Vec3f               projectionMtxFDiagonal;
    MtxF                billboardMtxF;
    void*               billboardMtx;
    u32                 gameplayFrames;
    u8                  unk_18844;
    u8                  haltAllActors;
    s16                 numSetupActors;
    u8                  numRooms;
    void*               roomList;
    void*               linkActorEntry;
    void*               setupActorList;
    void*               actorCsCamList;
    void*               setupEntranceList;
    u16*                setupExitList;
    Path*               setupPathList;
    void*               naviQuestHints;
    void*               sceneMaterialAnims;
    void*               specialEffects;
    u8                  skyboxId;
    u8                  transitionTrigger;
    s16                 worldCoverAlpha;
    s16                 bgCoverAlpha;
    u16                 nextEntrance;
    s8                  bButtonAmmoPlusOne;
    s8                  unk_1887d;
    s8                  unk_1887e;
    u8                  transitionType;
    /* 0x18880 */ u8 unk_18880;
    /* 0x18884 */ CollisionCheckContext colChkCtx;
    /* 0x18B20 */ u16 cutsceneFlags[20];
    /* 0x18B48 */ u8 curSpawn;
    /* 0x18B49 */ u8 unk_18B49;
    u8                  transitionMode;
    char                unk_18b4b[0x70d];
}
PlayState;

ASSERT_OFFSET(PlayState, colCtx,                   0x00830);
ASSERT_OFFSET(PlayState, actorCtx,                 0x01ca0);
ASSERT_OFFSET(PlayState, csCtx,                    0x01f24);
ASSERT_OFFSET(PlayState, sramCtx,                  0x046b8);
ASSERT_OFFSET(PlayState, msgCtx,                   0x04908);
ASSERT_OFFSET(PlayState, interfaceCtx,             0x169e8);
ASSERT_OFFSET(PlayState, envCtx,                   0x17004);
ASSERT_OFFSET(PlayState, roomCtx,                  0x186e0);
ASSERT_OFFSET(PlayState, gameplayFrames,           0x18840);
ASSERT_OFFSET(PlayState, setupExitList,            0x18860);
ASSERT_OFFSET(PlayState, transitionType,           0x1887f);
ASSERT_OFFSET(PlayState, transitionMode,           0x18b4a);

#define TRANS_TRIGGER_OFF          0x00
#define TRANS_TRIGGER_START        0x14
#define TRANS_TRIGGER_END         -0x14

_Static_assert(sizeof(PlayState) == 0x19258, "MM PlayState size is wrong");

typedef struct RegEditor {
    /* 0x00 */ u8  regPage; /* 0: no page selected (reg editor is not active); 1: first page; `REG_PAGES`: last page */
    /* 0x01 */ u8  regGroup; /* Indexed from 0 to `REG_GROUPS`-1. Each group has its own character to identify it. */
    /* 0x02 */ u8  regCur; /* Selected reg, indexed from 0 as the page start */
    /* 0x03 */ u8  dPadInputPrev;
    /* 0x04 */ u32 inputRepeatTimer;
    /* 0x08 */ s8 pad_08[0xC];
    /* 0x14 */ s16 data[REG_GROUPS * REG_PER_GROUP]; /* Accessed through *REG macros */
} RegEditor; /* size = 0x15D4 */

void Play_EnableMotionBlur(u32 alpha);
s32 Play_SetCameraFov(PlayState* this, s16 camId, f32 fov);
void Play_DisableMotionBlur(void);

#endif
