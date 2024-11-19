#ifndef MM_SAVE_H
#define MM_SAVE_H

#include <combo/types.h>
#include <combo/xflags.h>

typedef struct
{
    char    newf[6];
    u16     songOfTimeCount;
    u8      playerName[8];
    s16     healthCapacity;
    s16     health;
    s8      magicLevel;
    s8      magic;
    s16     rupees;
    u16     swordHealth;
    u16     tatlTimer;
    u8      isMagicAcquired;
    u8      isDoubleMagicAcquired;
    u8      doubleDefense;
    u8      unk_1F;
    u16     unk_20;
    u16     owlActivationFlags;
    u16     unk_24;
    s16     savedSceneNum;
}
MmSavePlayerData;

typedef struct
{
    u8      buttonItems[4][4];
    u8      cButtonSlots[4][4];
    u16     boots:4;
    u16     tunic:4;
    u16     shield:4;
    u16     sword:4;
}
MmItemEquips;

typedef struct
{
    u32     unused:9;
    u32     dekuNut:3;
    u32     dekuStick:3;
    u32     unused2:3; /* bullet bag */
    u32     wallet:2;
    u32     scale:3;
    u32     strength:3;
    u32     bombBag:3;
    u32     quiver:3;
}
MmUpgrades;

typedef struct
{
    u8 maxKeys:5;
    u8 map:1;
    u8 compass:1;
    u8 bossKey:1;
}
MmDungeonItems;

typedef union
{
    struct
    {
        u32 heartPieces:4;
        u32 unused:3;
        u32 songLullabyIntro:1;
        u32 unused2:5;
        u32 notebook:1;
        u32 songSun:1;
        u32 songStorms:1;
        u32 songSoaring:1;
        u32 songEpona:1;
        u32 songHealing:1;
        u32 songTime:1;
        u32 songSaria:1;
        u32 songOrder:1;
        u32 songEmpty:1;
        u32 songNewWave:1;
        u32 songLullaby:1;
        u32 songAwakening:1;
        u32 unused3:2;
        u32 remainsTwinmold:1;
        u32 remainsGyorg:1;
        u32 remainsGoht:1;
        u32 remainsOdolwa:1;
    };
    u32 value;
}
MmQuestItems;

#define QUEST_MM_REMAINS_ODOLWA         0
#define QUEST_MM_REMAINS_GOHT           1
#define QUEST_MM_REMAINS_GYORG          2
#define QUEST_MM_REMAINS_TWINMOLD       3
#define QUEST_MM_SONG_AWAKENING         6
#define QUEST_MM_SONG_GORON             7
#define QUEST_MM_SONG_ZORA              8
#define QUEST_MM_SONG_EMPTINESS         9
#define QUEST_MM_SONG_ORDER             10
#define QUEST_MM_SONG_SARIA             11
#define QUEST_MM_SONG_TIME              12
#define QUEST_MM_SONG_HEALING           13
#define QUEST_MM_SONG_EPONA             14
#define QUEST_MM_SONG_SOARING           15
#define QUEST_MM_SONG_STORMS            16
#define QUEST_MM_SONG_SUN               17
#define QUEST_MM_NOTEBOOK               18
#define QUEST_MM_SONG_GORON_HALF        24

typedef struct
{
    u8              items[48];
    s8              ammo[24];
    MmUpgrades      upgrades;
    MmQuestItems    quest;
    MmDungeonItems  dungeonItems[10];
    s8              dungeonKeys[9];
    s8              defenseHearts;
    s8              strayFairies[10];
    char            dekuPlaygroundPlayerName[3][8];
}
MmInventory;

typedef union
{
    struct
    {
        u32 chest;
        u32 switch0;
        u32 switch1;
        u32 clearedRoom;
        u32 collectible;
        u32 clearedFloors;
        u32 rooms;
    };
    char raw[0x1c];
}
MmPermanentSceneFlags;

typedef union
{
    struct
    {
        u32 chest;
        u32 switch0;
        u32 switch1;
        u32 clearedRoom;
        u32 collectible;
    };
    char raw[0x14];
}
MmCycleSceneFlags;

typedef struct
{
    s16         scene;
    Vec3s       pos;
    s16         yaw;
}
MmHorseData;

typedef struct {
    /* 0x00 */ Vec3f pos; /* Normally it's a Vec3i, but this is easier. */
    /* 0x0C */ s32 yaw;
    /* 0x10 */ s32 playerParams;
    /* 0x14 */ s32 entrance;
    /* 0x18 */ s32 roomIndex;
    /* 0x1C */ s32 set;
    /* 0x20 */ s32 tempSwchFlags;
    /* 0x24 */ s32 tempCollectFlags;
} FaroresWindData; /* size = 0x28 */

typedef struct RespawnData {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 yaw;
    /* 0x0E */ s16 playerParams;
    /* 0x10 */ u16 entrance;
    /* 0x12 */ u8 roomIndex;
    /* 0x13 */ s8 data;
    /* 0x14 */ u32 tempSwitchFlags;
    /* 0x18 */ u32 unk_18;
    /* 0x1C */ u32 tempCollectFlags;
} RespawnData; /* size = 0x20 */

typedef struct
{
    MmSavePlayerData        playerData;
    MmItemEquips            itemEquips;
    MmInventory             inventory;
    MmPermanentSceneFlags   permanentSceneFlags[120];
    u8                      unk_DF4[0x54];
    u32                     dekuPlaygroundHighScores[3];
    u32                     pictoFlags0;
    u32                     pictoFlags1;
    u32                     unk_E80;
    u32                     unk_E84;
    u32                     unk_E88[7];
    u32                     scenesVisible[7];
    u16                     skullCountSwamp;
    u16                     skullCountOcean;
    u32                     unk_EC4;
    u32                     unk_EC8;
    u32                     unk_ECC[2];
    u32                     stolenItems;
    u32                     unk_ED8;
    u32                     bankRupees;
    u32                     unk_ee0;
    u32                     unk_ee4;
    u32                     unk_ee8;
    u32                     horseBackBalloonHighScore;
    u32                     lotteryCodeGuess;
    u16                     shootingGalleryHighScoreSwamp;
    u16                     shootingGalleryHighScoreTown;
    u8                      weekEventReg[100];
    u32                     mapsVisited;
    u32                     worldMapCloudVisibility;
    u8                      unk_F64;
    u8                      unk_F65;
    u8                      scarecrowSpawnSong[128];
    s8                      bombersCaughtNum;
    s8                      bombersCaughtOrder[5];
    s8                      lotteryCodes[3][3];
    s8                      spiderHouseMaskOrder[6];
    s8                      bomberCode[5];
    MmHorseData             horseData;
    u16                     checksum;
}
MmSaveInfo;

typedef struct
{
    s32                     entrance;
    u8                      equippedMask;
    u8                      isFirstCycle;
    char                    unk_006;
    u8                      linkAge;
    s32                     cutscene;
    u16                     time;
    u16                     owlLocation;
    s32                     isNight;
    s32                     daySpeed;
    u32                     day;
    u32                     daysElapsed;
    u8                      playerForm;
    u8                      snowheadCleared;
    u8                      hasTatl;
    u8                      isOwlSave;
    MmSaveInfo              info;
    u8                      eventInf[8];
    u8                      hasSirloin;
    u8                      unk_1015;
    u16                     jinxTimer;
    s16                     rupeesDelta;
    char                    unk[0x2c86];
}
MmSave;

ASSERT_SIZE(MmSave, 0x3ca0);
ASSERT_OFFSET(MmSave, entrance,    0x0000);
ASSERT_OFFSET(MmSave, equippedMask,     0x0004);
ASSERT_OFFSET(MmSave, isFirstCycle,     0x0005);
ASSERT_OFFSET(MmSave, unk_006,          0x0006);
ASSERT_OFFSET(MmSave, linkAge,          0x0007);
ASSERT_OFFSET(MmSave, cutscene,         0x0008);
ASSERT_OFFSET(MmSave, time,             0x000c);

typedef struct
{
    u16 optionId;
    u8  unk_02;
    u8  audio;
    u8  unk_04;
    u8  zTarget;
}
SaveOptions;

typedef enum {
    /*  0 */ TIMER_ID_POSTMAN, /* postman's counting minigame */
    /*  1 */ TIMER_ID_MINIGAME_1, /* minigame timer */
    /*  2 */ TIMER_ID_2,
    /*  3 */ TIMER_ID_MOON_CRASH, /* timer used for mooncrash on the clocktower roof */
    /*  4 */ TIMER_ID_MINIGAME_2, /* minigame timer */
    /*  5 */ TIMER_ID_ENV_HAZARD, /* environmental hazard timer (underwater or hot room) */
    /*  6 */ TIMER_ID_GORON_RACE_UNUSED,
    /*  7 */ TIMER_ID_MAX,
    /* 99 */ TIMER_ID_NONE = 99
} TimerId;

typedef enum {
    /*  0 */ MAGIC_STATE_IDLE, // Regular gameplay
    /*  1 */ MAGIC_STATE_CONSUME_SETUP, // Sets the speed at which the magic border flashes
    /*  2 */ MAGIC_STATE_CONSUME, // Consume magic until target is reached or no more magic is available
    /*  3 */ MAGIC_STATE_METER_FLASH_1, // Flashes border
    /*  4 */ MAGIC_STATE_METER_FLASH_2, // Flashes border and draws yellow magic to preview target consumption
    /*  5 */ MAGIC_STATE_RESET, // Reset colors and return to idle
    /*  6 */ MAGIC_STATE_METER_FLASH_3, // Flashes border with no additional behaviour
    /*  7 */ MAGIC_STATE_CONSUME_LENS, // Magic slowly consumed by Lens of Truth
    /*  8 */ MAGIC_STATE_STEP_CAPACITY, // Step `magicCapacity` to full capacity
    /*  9 */ MAGIC_STATE_FILL, // Add magic until magicFillTarget is reached
    /* 10 */ MAGIC_STATE_CONSUME_GORON_ZORA_SETUP,
    /* 11 */ MAGIC_STATE_CONSUME_GORON_ZORA, // Magic slowly consumed by Goron spiked rolling or Zora electric barrier.
    /* 12 */ MAGIC_STATE_CONSUME_GIANTS_MASK // Magic slowly consumed by Giant's Mask
} MagicState;

typedef enum {
    /* 0 */ MAGIC_CONSUME_NOW, // Consume magic immediately without preview
    /* 1 */ MAGIC_CONSUME_WAIT_NO_PREVIEW, // Sets consume target but waits to consume. No yellow magic preview to target consumption. Unused
    /* 2 */ MAGIC_CONSUME_NOW_ALT, // Identical behaviour to MAGIC_CONSUME_NOW. Unused
    /* 3 */ MAGIC_CONSUME_LENS, // Lens of Truth consumption
    /* 4 */ MAGIC_CONSUME_WAIT_PREVIEW, // Sets consume target but waits to consume. Show magic to be consumed in yellow.
    /* 5 */ MAGIC_CONSUME_GORON_ZORA, // Goron spiked rolling or Zora electric barrier slow consumption
    /* 6 */ MAGIC_CONSUME_GIANTS_MASK, // Giant's Mask slow consumption
    /* 7 */ MAGIC_CONSUME_DEITY_BEAM // Fierce Deity Beam consumption, consumed magic now and not via request
} MagicChangeType;

typedef struct
{
    /* 0x0000 */ MmSave save;
    /* 0x3CA0 */ u32 fileNum;
    /* 0x3CA4 */ s16 powderKegTimer;                    /* "big_bom_timer" */
    /* 0x3CA6 */ u8 unk_3CA6;
    /* 0x3CA7 */ u8 unk_3CA7;                           /* "day_night_flag" */
    /* 0x3CA8 */ s32 gameMode;
    /* 0x3CAC */ s32 sceneSetupId;
    /* 0x3CB0 */ s32 respawnFlag;                       /* "restart_flag" */
    /* 0x3CB4 */ RespawnData respawn[RESPAWN_MODE_MAX]; /* "restart_data" */
    /* 0x3DB4 */ f32 entranceSpeed;                     /* "player_wipe_speedF" */
    /* 0x3DB8 */ u16 entranceSound;                     /* "player_wipe_door_SE" */
    /* 0x3DBA */ u8 unk_3DBA;                           /* "player_wipe_item" */
    /* 0x3DBB */ u8 retainWeatherMode;                  /* "next_walk" */
    /* 0x3DBC */ s16 dogParams;                         /* OoT leftover. "dog_flag" */
    /* 0x3DBE */ u8 envHazardTextTriggerFlags;          /* "guide_status" */
    /* 0x3DBF */ u8 showTitleCard;                      /* "name_display" */
    /* 0x3DC0 */ s16 nayrusLoveTimer;                   /* remnant of OoT, "shield_magic_timer" */
    /* 0x3DC2 */ u8 unk_3DC2;                           /* "pad1" */
    /* 0x3DC8 */ OSTime postmanTimerStopOsTime;         /* The osTime when the timer stops for the postman minigame. "get_time" */
    /* 0x3DD0 */ u8 timerStates[TIMER_ID_MAX];          /* See the `TimerState` enum. "event_fg" */
    /* 0x3DD7 */ u8 timerDirections[TIMER_ID_MAX];      /* See the `TimerDirection` enum. "calc_flag" */
    /* 0x3DE0 */ u64 timerCurTimes[TIMER_ID_MAX];       /* For countdown, the remaining time left. For countup, the time since the start. In centiseconds (1/100th sec). "event_ostime" */
    /* 0x3E18 */ u64 timerTimeLimits[TIMER_ID_MAX];     /* The original total time given for the timer to count from, in centiseconds (1/100th sec). "event_sub" */
    /* 0x3E50 */ OSTime timerStartOsTimes[TIMER_ID_MAX]; /* The osTime when the timer starts. "func_time" */
    /* 0x3E88 */ u64 timerStopTimes[TIMER_ID_MAX];       /* The total amount of time taken between the start and end of the timer, in centiseconds (1/100th sec). "func_end_time" */
    /* 0x3EC0 */ OSTime timerPausedOsTimes[TIMER_ID_MAX]; /* The cumulative osTime spent with the timer paused. "func_stop_time" */
    /* 0x3EF8 */ s16 timerX[TIMER_ID_MAX];              /* "event_xp" */
    /* 0x3F06 */ s16 timerY[TIMER_ID_MAX];              /* "event_yp" */
    /* 0x3F14 */ s16 unk_3F14;                          /* "character_change" */
    /* 0x3F16 */ u8 seqId;                              /* "old_bgm" */
    /* 0x3F17 */ u8 ambienceId;                         /* "old_env" */
    /* 0x3F18 */ u8 buttonStatus[6];                    /* "button_item" */
    /* 0x3F1E */ u8 hudVisibilityForceButtonAlphasByStatus; /* if btn alphas are updated through Interface_UpdateButtonAlphas, instead update them through Interface_UpdateButtonAlphasByStatus "ck_fg" */
    /* 0x3F20 */ u16 nextHudVisibilityMode;             /* triggers the hud to change visibility to the requested value. Reset to HUD_VISIBILITY_IDLE when target is reached "alpha_type" */
    /* 0x3F22 */ u16 hudVisibility;                     /* current hud visibility "prev_alpha_type" */
    /* 0x3F24 */ u16 hudVisibilityTimer;                /* number of frames in the transition to a new hud visibility. Used to step alpha "alpha_count" */
    /* 0x3F26 */ u16 prevHudVisibilityMode;             /* used to store and recover hud visibility for pause menu and text boxes "last_time_type" */
    u16                 magicState;
    u16                 isMagicRequested;
    u16                 magicFlag;
    u16                 magicCapacity;
    u16                 magicFillTarget;
    u16                 magicToConsume;
    u16                 magicToAdd;
    u16                 dungeonId2;
    u16                 minigameState;
    u16                 minigameScore;
    u16                 minigameHiddenScore;
    char                unk_3f3e[0x2];
    SaveOptions         options;
    char                unk_3f46[0x4];
    u16                 nextCutscene;
    u8                  cutsceneTrigger;
    u8                  chamberCutsceneNum;
    u16                 nextDayTime;
    u8                  transFadeDuration;
    u8                  transWipeSpeed;
    u16                 skyboxTime;
    u8                  dogIsLost;
    u8                  nextTransitionType;
    s16                 worldMapArea;
    s16                 sunsSongState;
    u16                 healthDelta;
    s32                 betRupees;
    u8                  screenScaleFlag;
    f32                 screenScale;
    MmCycleSceneFlags   cycleSceneFlags[120];
    u16                 dungeonId;
}
MmSaveContext;

_Static_assert(sizeof(MmSaveContext) == 0x48d0, "MmSaveContext size is wrong");

ASSERT_OFFSET(MmSaveContext, fileNum,               0x3ca0);
ASSERT_OFFSET(MmSaveContext, gameMode,              0x3ca8);
ASSERT_OFFSET(MmSaveContext, sceneSetupId,          0x3cac);
ASSERT_OFFSET(MmSaveContext, timerStates,           0x3dd0);
ASSERT_OFFSET(MmSaveContext, timerDirections,       0x3dd7);
ASSERT_OFFSET(MmSaveContext, dungeonId2,            0x3f36);
ASSERT_OFFSET(MmSaveContext, minigameState,         0x3f38);
ASSERT_OFFSET(MmSaveContext, minigameScore,         0x3f3a);
ASSERT_OFFSET(MmSaveContext, minigameHiddenScore,   0x3f3c);
ASSERT_OFFSET(MmSaveContext, options,               0x3f40);
ASSERT_OFFSET(MmSaveContext, nextCutscene,          0x3f4a);
ASSERT_OFFSET(MmSaveContext, cutsceneTrigger,       0x3f4c);
ASSERT_OFFSET(MmSaveContext, healthDelta,           0x3f5a);
ASSERT_OFFSET(MmSaveContext, betRupees,             0x3f5c);
ASSERT_OFFSET(MmSaveContext, cycleSceneFlags,       0x3f68);
ASSERT_OFFSET(MmSaveContext, dungeonId,             0x48c8);

#define HUD_VISIBILITY_IDLE                                     0
#define HUD_VISIBILITY_NONE                                     1
#define HUD_VISIBILITY_NONE_ALT                                 2
#define HUD_VISIBILITY_HEARTS_WITH_OVERWRITE                    3
#define HUD_VISIBILITY_A                                        4
#define HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE            5
#define HUD_VISIBILITY_A_HEARTS_MAGIC_MINIMAP_WITH_OVERWRITE    6
#define HUD_VISIBILITY_ALL_NO_MINIMAP_W_DISABLED                7
#define HUD_VISIBILITY_B                                        8
#define HUD_VISIBILITY_HEARTS_MAGIC                             9
#define HUD_VISIBILITY_B_ALT                                    10
#define HUD_VISIBILITY_HEARTS                                   11
#define HUD_VISIBILITY_A_B_MINIMAP                              12
#define HUD_VISIBILITY_HEARTS_MAGIC_WITH_OVERWRITE              13
#define HUD_VISIBILITY_HEARTS_MAGIC_C                           14
#define HUD_VISIBILITY_ALL_NO_MINIMAP                           15
#define HUD_VISIBILITY_A_B_C                                    16
#define HUD_VISIBILITY_B_MINIMAP                                17
#define HUD_VISIBILITY_HEARTS_MAGIC_MINIMAP                     18
#define HUD_VISIBILITY_A_HEARTS_MAGIC_MINIMAP                   19
#define HUD_VISIBILITY_B_MAGIC                                  20
#define HUD_VISIBILITY_A_B                                      21
#define HUD_VISIBILITY_A_B_HEARTS_MAGIC_MINIMAP                 22
#define HUD_VISIBILITY_ALL                                      50
#define HUD_VISIBILITY_NONE_INSTANT                             52

#if defined(GAME_MM)
ALIGNED(16) extern MmSaveContext gSaveContext;
# define gMmSave        (gSaveContext.save)
# define gSave          gMmSave
# define gForeignSave   gOotSave
#else
ALIGNED(16) extern MmSave gMmSave;
#endif

/* Custom */
typedef struct
{
    u8 boss;
    u8 bossCycle;
    u8 items;
    u8 unused;
}
MmExtraBoss;

typedef struct
{
    u32 hookshot:2;
    u32 ocarina:2;
    u32 goldDust:1;
    u32 hammerGFS:2;
    u32 unused:25;
}
MmExtraItems;

typedef struct
{
    u32 trade1:6;
    u32 trade2:5;
    u32 trade3:5;
    u32 tradeObtained1:6;
    u32 tradeObtained2:5;
    u32 tradeObtained3:5;
}
MmExtraTrade;

typedef struct
{
    u32 pictobox:1;
    u32 songSoaring:1;
    u32 maskBunny:1;
    u32 songEpona:1;
    u32 maskRomani:1;
    u32 maskGaro:1;
    u32 maskZora:1;
    u32 maskDonGero:1;
    u32 swordRazor:1;
    u32 swordGilded:1;
    u32 maskGoron:1;
    u32 scrubTown:1;
    u32 scrubSwamp:1;
    u32 scrubMountain:1;
    u32 scrubOcean:1;
    u32 scrubValley:1;
    u32 scrubBombBag:1;
    u32 zoraHallLights:1;
    u32 songGoronIntro:1;
    u32 songGoron:1;
    u32 powderKeg:1;
    u32 maskStone:1;
    u32 songStorms:1;
    u32 songEmptiness:1;
    u32 maskGibdo:1;
    u32 greatFairies:6;
    u32 bankReward2:1;
}
MmExtraFlags;

typedef struct
{
    u32 childWallet:1;
    u32 scrubBeans:1;
    u32 scrubPotionGreen:1;
    u32 maskKafei:1;
    u32 honeyDarling:1;
    u32 roomKey:1;
    u32 letterKafei:1;
    u32 pendant:1;
    u32 letterMama:1;
    u32 notebook:1;
    u32 maskBlast:1;
    u32 dekuPlayground:1;
    u32 maskCouple:1;
    u32 grandma:1;
    u32 maskPostman:1;
    u32 maskTroupeLeader:1;
    u32 maskFierceDeity:1;
    u32 ocarina:1;
    u32 songOath:1;
    u32 progressiveShield:1;
    u32 majora:1;
    u32 maskBremen:1;
    u32 maskScents:1;
    u32 maskKamaro:1;
    u32 maskTruth:1;
    u32 moonTear:1;
    u32 songHealing:1;
    u32 townStrayFairy:1;
    u32 scrubPotionBlue:1;
    u32 gormanMilk:1;
    u32 milkBarMilk:1;
    u32 milkBarChateau:1;
}
MmExtraFlags2;

typedef struct
{
    u32 bottomlessWallet:1;
    u32 stoneAgony:1;
    u32 unused:30;
}
MmExtraFlags3;

#define MM_PLAYER_FORM_FIERCE_DEITY 0
#define MM_PLAYER_FORM_GORON        1
#define MM_PLAYER_FORM_ZORA         2
#define MM_PLAYER_FORM_DEKU         3
#define MM_PLAYER_FORM_HUMAN        4

typedef struct ALIGNED(16)
{
    u8 xflags[XFLAGS_COUNT_MM];
    u8 npc[32];
    u8 shops[4];
    u8 halfDays;
    RespawnData fw[2];
    RespawnData fwRespawnTop[2];
    RespawnData fwRespawnDungeonEntrance[2];
}
MmCustomSave;

#endif /* MM_SAVE_H */
