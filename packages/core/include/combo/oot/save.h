#ifndef OOT_SAVE_H
#define OOT_SAVE_H

#include <combo/types.h>
#include <combo/util.h>
#include <combo/xflags.h>

typedef struct
{
    char        newf[6];
    s16         deathCount;
    u8          playerName[8];
    s16         ddOnlyFlag;
    s16         healthMax;
    s16         health;
    s8          magicSize;
    s8          magicAmount;
    u16         rupees;
    u16         swordHealth;
    char        unk_38[2];
    u8          magicUpgrade;
    char        unk_3b[1];
    u8          magicUpgrade2;
    u8          doubleDefense;
}
OotSavePlayerData;

typedef union
{
    struct
    {
        u32 chests;
        u32 switches;
        u32 roomClear;
        u32 collectibles;
        u32 unused;
        u32 visitedRooms;
        u32 visitedFloors;
    };
    char raw[0x1c];
}
OotPermanentSceneFlags;

typedef struct
{
    u16  boots:4;
    u16  tunics:4;
    u16  shields:4;
    u16  swords:4;
}
OotEquipment;

typedef struct
{
    u8              buttonItems[4];
    u8              cButtonSlots[3];
    OotEquipment    equipment;
}
OotItemEquips;

_Static_assert(sizeof(OotItemEquips) == 0xa, "Wrong size for OotItemEquips");

typedef struct
{
    u32  unused:9;
    u32  dekuNut:3;
    u32  dekuStick:3;
    u32  bulletBag:3;
    u32  wallet:2;
    u32  dive:3;
    u32  strength:3;
    u32  bombBag:3;
    u32  quiver:3;
}
OotSaveUpgrades;

typedef union
{
    struct {
        u32  heartPieces:4;
        u32  unused:4;
        u32  goldToken:1;
        u32  gerudoCard:1;
        u32  agonyStone:1;
        u32  stoneSapphire:1;
        u32  stoneRuby:1;
        u32  stoneEmerald:1;
        u32  songStorms:1;
        u32  songTime:1;
        u32  songSun:1;
        u32  songSaria:1;
        u32  songEpona:1;
        u32  songZelda:1;
        u32  songTpLight:1;
        u32  songTpShadow:1;
        u32  songTpSpirit:1;
        u32  songTpWater:1;
        u32  songTpFire:1;
        u32  songTpForest:1;
        u32  medallionLight:1;
        u32  medallionShadow:1;
        u32  medallionSpirit:1;
        u32  medallionWater:1;
        u32  medallionFire:1;
        u32  medallionForest:1;
    };
    u32 value;
}
OotSaveQuest;

#define QUEST_OOT_MEDALLION_FOREST  0
#define QUEST_OOT_MEDALLION_FIRE    1
#define QUEST_OOT_MEDALLION_WATER   2
#define QUEST_OOT_MEDALLION_SPIRIT  3
#define QUEST_OOT_MEDALLION_SHADOW  4
#define QUEST_OOT_MEDALLION_LIGHT   5
#define QUEST_OOT_SONG_TP_FOREST    6
#define QUEST_OOT_SONG_TP_FIRE      7
#define QUEST_OOT_SONG_TP_WATER     8
#define QUEST_OOT_SONG_TP_SPIRIT    9
#define QUEST_OOT_SONG_TP_SHADOW    10
#define QUEST_OOT_SONG_TP_LIGHT     11
#define QUEST_OOT_SONG_ZELDA        12
#define QUEST_OOT_SONG_EPONA        13
#define QUEST_OOT_SONG_SARIA        14
#define QUEST_OOT_SONG_SUN          15
#define QUEST_OOT_SONG_TIME         16
#define QUEST_OOT_SONG_STORMS       17
#define QUEST_OOT_STONE_EMERALD     18
#define QUEST_OOT_STONE_RUBY        19
#define QUEST_OOT_STONE_SAPPHIRE    20
#define QUEST_OOT_STONE_AGONY       21
#define QUEST_OOT_GERUDO_CARD       22
#define QUEST_OOT_GOLD_TOKEN        23

typedef struct
{
    u8 maxKeys:5;
    u8 map:1;
    u8 compass:1;
    u8 bossKey:1;
}
OotDungeonItems;

typedef struct
{
    u8                      items[0x18];
    u8                      ammo[0xf];
    u8                      beans;
    OotEquipment            equipment;
    u16                     unk_9e;
    OotSaveUpgrades         upgrades;
    OotSaveQuest            quest;
    OotDungeonItems         dungeonItems[0x14];
    s8                      dungeonKeys[0x13];
    u8                      doubleDefenseHearts;
    u16                     goldTokens;
}
OotInventory;

typedef struct
{
    Vec3i   pos;
    s32     yaw;
    s32     playerParams;
    s32     entranceIndex;
    s32     roomIndex;
    s32     set;
    s32     tempSwitchFlags;
    s32     tempCollectFlags;
}
OotFaroreWind;

_Static_assert(sizeof(OotFaroreWind) == 0x28, "OotFaroreWind size is wrong");

typedef struct
{
    s16 sceneId;
    Vec3s pos;
    s16 angle;
}
OotHorseData;

_Static_assert(sizeof(OotHorseData) == 0x0a, "OotHorseData size is wrong");

typedef struct
{
    u32                     entrance;
    u32                     age;
    u32                     cutscene;
    u16                     worldTime;
    u16                     unk_0e;
    u32                     isNight;
    char                    unk_14[0x4];
    u32                     tradeQuestFlag;
    OotSavePlayerData       playerData;
    u8                      isBiggoronSword;
    u8                      ocarinaGameRound;
    OotItemEquips           childEquips;
    OotItemEquips           adultEquips;
    char                    unk_54[0x12];
    u16                     sceneId;
    OotItemEquips           equips;
    char                    unk_72[0x2];
    OotInventory            inventory;
    OotPermanentSceneFlags  perm[124];
    OotFaroreWind           fw;
    char                    unk_e8c[0x10];
    s32                     gsFlags[6];
    char                    unk_EB4[0x4];
    s32                     highScores[7];
    u16                     eventsChk[14];
    u16                     eventsItem[4];
    u16                     eventsMisc[30];
    char                    unk_f34[0x4];
    u32                     worldMapAreaData;
    char                    unk_f3c[0x4];
    u8                      scarecrowLongSongSet;
    u8                      scarecrowLongSong[0x360];
    char                    unk_12a1[0x24];
    u8                      scarecrowSpawnSongSet;
    u8                      scarecrowSpawnSong[0x80];
    char                    unk_1346[0x02];
    OotHorseData            horseData;
    u16                     checksum;
}
OotSave;

ASSERT_OFFSET(OotSave, childEquips,             0x40);
ASSERT_OFFSET(OotSave, adultEquips,             0x4a);
ASSERT_OFFSET(OotSave, unk_54,                  0x54);
ASSERT_OFFSET(OotSave, sceneId,                 0x66);
ASSERT_OFFSET(OotSave, equips,                  0x68);
ASSERT_OFFSET(OotSave, inventory.equipment,     0x9c);
ASSERT_OFFSET(OotSave, perm,                    0xd4);
ASSERT_OFFSET(OotSave, unk_f34,                 0xf34);
ASSERT_OFFSET(OotSave, unk_f3c,                 0xf3c);
ASSERT_OFFSET(OotSave, unk_12a1,                0x12a1);
ASSERT_OFFSET(OotSave, unk_1346,                0x1346);
ASSERT_OFFSET(OotSave, horseData,               0x1348);
ASSERT_OFFSET(OotSave, checksum,                0x1352);

typedef struct
{
    OotSave save;
    u32     fileIndex;
    char    unk_1358[0x04];
    s32     gameMode;
    s32     sceneSetupId;
    s32     respawnFlag;
    char    unk_1368[0x2f];
    u8      grottoChestFlag;
    char    unk_1398[0x34];
    s16     rupeesDelta;
    /* 0x13CE */ s16 timerState; // See `TimerState`
    /* 0x13D0 */ s16 timerSeconds;
    /* 0x13D2 */ s16 subTimerState; // See `SubTimerState`
    /* 0x13D4 */ s16 subTimerSeconds;
    /* 0x13D6 */ s16 timerX[2]; // TIMER_ID_MAX
    /* 0x13DA */ s16 timerY[2]; // TIMER_ID_MAX
    /* 0x13DE */ char unk_13DE[0x0002];
    /* 0x13E0 */ u8 seqId;
    /* 0x13E1 */ u8 natureAmbienceId;
    /* 0x13E2 */ u8 buttonStatus[5];
    /* 0x13E7 */ u8 forceRisingButtonAlphas; // if btn alphas are updated through Interface_DimButtonAlphas, instead update them through Interface_RaiseButtonAlphas
    /* 0x13E8 */ u16 nextHudVisibilityMode; // triggers the hud to change visibility mode to the requested value. Reset to HUD_VISIBILITY_NO_CHANGE when target is reached
    /* 0x13EA */ u16 hudVisibilityMode; // current hud visibility mode
    /* 0x13EC */ u16 hudVisibilityModeTimer; // number of frames in the transition to a new hud visibility mode. Used to step alpha
    /* 0x13EE */ u16 prevHudVisibilityMode; // used to store and recover hud visibility mode for pause menu and text boxes
    /* 0x13F0 */ s16 magicState; // determines magic meter behavior on each frame
    /* 0x13F2 */ s16 prevMagicState; // used to resume the previous state after adding or filling magic
    /* 0x13F4 */ s16 magicCapacity; // maximum magic available
    /* 0x13F6 */ s16 magicFillTarget; // target used to fill magic. Target can either be full capacity (Magic_Fill, magic upgrades), or the saved magic amount (loading a file, game over)
    /* 0x13F8 */ s16 magicTarget; // target for magic to step to when adding or consuming magic
    u16     eventInf[4];
    u16     mapIndex;
    u16     minigameState;
    u16     minigameScore;
    char    unk_1408[1];
    u8      language;
    u8      audioSetting;
    char    unk_140b[1];
    u8      zTargetSetting;
    u16     forcedSeqId;
    u8      cutsceneTransitionControl;
    char    unk_1411[1];
    u16     nextCutscene;
    u8      inCutscene;
    char    unk_1415[0x0f];
    u16     healthDelta;
    char    unk_1426[0x2a];
}
OotSaveContext;

ASSERT_OFFSET(OotSaveContext, gameMode,                     0x135c);
ASSERT_OFFSET(OotSaveContext, sceneSetupId,                 0x1360);
ASSERT_OFFSET(OotSaveContext, unk_1368,                     0x1368);
ASSERT_OFFSET(OotSaveContext, grottoChestFlag,              0x1397);
ASSERT_OFFSET(OotSaveContext, unk_1398,                     0x1398);
ASSERT_OFFSET(OotSaveContext, mapIndex,                     0x1402);
ASSERT_OFFSET(OotSaveContext, minigameState,                0x1404);
ASSERT_OFFSET(OotSaveContext, minigameScore,                0x1406);
ASSERT_OFFSET(OotSaveContext, unk_1408,                     0x1408);
ASSERT_OFFSET(OotSaveContext, language,                     0x1409);
ASSERT_OFFSET(OotSaveContext, audioSetting,                 0x140a);
ASSERT_OFFSET(OotSaveContext, unk_140b,                     0x140b);
ASSERT_OFFSET(OotSaveContext, zTargetSetting,               0x140c);
ASSERT_OFFSET(OotSaveContext, forcedSeqId,                  0x140e);
ASSERT_OFFSET(OotSaveContext, cutsceneTransitionControl,    0x1410);
ASSERT_OFFSET(OotSaveContext, unk_1411,                     0x1411);
ASSERT_OFFSET(OotSaveContext, nextCutscene,                 0x1412);
ASSERT_OFFSET(OotSaveContext, inCutscene,                   0x1414);
ASSERT_OFFSET(OotSaveContext, healthDelta,                  0x1424);
ASSERT_OFFSET(OotSaveContext, unk_1426,                     0x1426);

_Static_assert(sizeof(OotSave) == 0x1354, "OotSave size is wrong");
_Static_assert(sizeof(OotSaveContext) == 0x1450, "OotSaveContext size is wrong");

#if defined(GAME_OOT)
ALIGNED(16) extern OotSaveContext gSaveContext;
# define gOotSave       (gSaveContext.save)
# define gSave          gOotSave
# define gForeignSave   gMmSave
#else
ALIGNED(16) extern OotSave gOotSave;
#endif

/* Custom */
typedef struct
{
    u16 child;
    u16 adult;
}
OotExtraTrade;

typedef struct
{
    u8 rutoLetter:1;
    u8 unused:7;
    u8 hookshot;
    u8 shield;
    u8 ocarina;
}
OotExtraItems;

typedef struct
{
    u32 ganon:1;
    u32 greatFairies:6;
    u32 fishingChild:1;
    u32 fishingAdult:1;
    u32 tunicGoron:1;
    u32 biggoron:1;
    u32 tunicZora:1;
    u32 fireArrow:1;
    u32 ganonBossKey:1;
    u32 childWallet:1;
    u32 triforceWin:1;
    u32 endgameItemIsWin:1;
    u32 unused2:7;
    u32 bottomlessWallet:1;
    u32 chestGameKey:1;
    u32 unused:6;
}
OotExtraFlags;

typedef struct ALIGNED(16)
{
    u8  xflags[XFLAGS_COUNT_OOT];
    u8  npc[32];
    u8  shops[8];
    u8  scrubs[8];
    u8  sr[16];
    u8  hasElegy:1;
}
OotCustomSave;

#endif
