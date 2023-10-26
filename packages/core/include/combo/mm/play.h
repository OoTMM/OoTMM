#ifndef MM_PLAY_H
#define MM_PLAY_H

#include <combo/util.h>
#include <combo/game_state.h>
#include <combo/common/actor.h>
#include <combo/common/ocarina.h>

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
    char unk_000[0x222];
    s16 unk_222;
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

typedef struct ActorContext
{
    char        unk_000[0xf];
    ActorList   actors[12];
    char        unk_a0[0x1b4];
    Actor*      elegyStatues[4];
    char        unk_264[0x20];
}
ActorContext;

ASSERT_OFFSET(ActorContext, actors,         0x010);
ASSERT_OFFSET(ActorContext, elegyStatues,   0x254);
ASSERT_OFFSET(ActorContext, unk_264,        0x264);

_Static_assert(sizeof(ActorContext) == 0x284,       "MM ActorContext size is wrong");
_Static_assert(sizeof(EnvironmentContext) == 0x100, "MM EnvironmentContext size is wrong");

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
        char    schar[1280]; // msgBuf
        u16     wchar[640];  // msgBufWide
        u64     force_structure_alignment_msg;
    } textBuffer;
    u8*         messageStart;
    u8*         messageEnd;
    u8          unk_11d88; // current Char Buffer ?
}
Font; // size = 0x11D90

_Static_assert(sizeof(Font) == 0x11d90, "MM Font size is wrong");

typedef struct MessageTableEntry
{
    u16 textId;
    u8 typePos;
    const char* segment;
}
MessageTableEntry; // size = 0x8;

_Static_assert(sizeof(MessageTableEntry) == 0x8, "MM MessageTableEntry size is wrong");

typedef struct {
    s8      recentNote;
    s8      storedSong;
    s8      noteIndex;
    char    pad3;
    s8      playbackRecentNote;
    u8      playbackState; // 1 while doing playback, is reset to 0 to show the "You Played X song" text.
    u8      playbackNoteIndex;
    char    pad7;
} SongFrameInfo;

// Structure with some song state. Usually located at: 0x801FD43A.
typedef struct {
    SongFrameInfo   frameInfo[2];
    u16             frameCount;
    s16             analogAngle; // Angle of analog stick, modifies sound.
    char            pad14[0x14];
    u8              hasPlayedNote; // 1 if has played note since using ocarina.
    char            pad29[0x7];
    u16             flags; // 0x37DF if all songs.
    u8              noteIndex3;
    char            pad33;
} SongInfo;

_Static_assert(sizeof(SongFrameInfo) == 0x8, "MM SongFrameInfo size is wrong");
_Static_assert(sizeof(SongInfo) == 0x34, "MM SongInfo size is wrong");

typedef struct MessageContext
{
    char                        view[0x00168]; // TODO: Add actual View structure
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
    s16                         unk_12018; // messageR
    s16                         unk_1201a; // messageG
    s16                         unk_1201c; // messageB
    s16                         unk_1201e; // messageA
    u8                          unk_12020;  // probably textboxEndType
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
    u8                          unk_12048; // EnKakasi
    char                        unk_12049[0x1];
    s16                         unk_1204a[0x5];
    s16                         unk_12054[6]; // First, second and third digits in lottery code guess
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
    f32                         unk_12098; // Text_Scale?
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
    s8 id;
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


typedef struct GameState_Play
{
    GameState           gs;
    u16                 sceneId;
    char                unk_000a6[0x01bfa];
    ActorContext        actorCtx;
    char                unk_01f24[0x02794];
    SramContext         sramCtx;
    char                unk_046e0[0x228];
    MessageContext      msgCtx;
    InterfaceContext    interfaceCtx;
    PauseContext        pauseCtx;
    GameOverContext     gameOverCtx;
    EnvironmentContext  envCtx;
    char                unk_17104[0x015dc];
    RoomContext         roomCtx;
    char                unk_18760[0x3c];
    s16                 playerActorCsIds[10];
    float               viewProjectionMtxF[16];
    Vec3f               projectionMtxFDiagonal;
    float               billboardMtxF[16];
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
    void*               setupPathList;
    void*               naviQuestHints;
    void*               sceneMaterialAnims;
    void*               specialEffects;
    u8                  skyboxId;
    u8                  transitionTrigger;
    s16                 worldCoverAlpha;
    s16                 bgCoverAlpha;
    u16                 nextEntrance;
    s8                  unk_1887c;
    s8                  unk_1887d;
    s8                  unk_1887e;
    u8                  transitionType;
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

ASSERT_OFFSET(GameState_Play, actorCtx,                 0x01ca0);
ASSERT_OFFSET(GameState_Play, unk_01f24,                0x01f24);
ASSERT_OFFSET(GameState_Play, sramCtx,                  0x046b8);
ASSERT_OFFSET(GameState_Play, msgCtx,                   0x04908);
ASSERT_OFFSET(GameState_Play, interfaceCtx,             0x169e8);
ASSERT_OFFSET(GameState_Play, envCtx,                   0x17004);
ASSERT_OFFSET(GameState_Play, roomCtx,                  0x186e0);
ASSERT_OFFSET(GameState_Play, gameplayFrames,           0x18840);
ASSERT_OFFSET(GameState_Play, setupExitList,            0x18860);
ASSERT_OFFSET(GameState_Play, transitionType,           0x1887f);
ASSERT_OFFSET(GameState_Play, transitionMode,           0x18b4a);

#define TRANS_TRIGGER_NONE          0x00
#define TRANS_TRIGGER_NORMAL        0x14

#define TRANS_TYPE_BLACK     0x02

_Static_assert(sizeof(GameState_Play) == 0x19258, "MM GameState_Play size is wrong");

#endif
