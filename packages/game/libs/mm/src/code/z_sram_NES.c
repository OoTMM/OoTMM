#include "z64save.h"
#include "global.h"
#include "z64horse.h"

typedef struct PersistentCycleSceneFlags {
    /* 0x0 */ u32 switch0;
    /* 0x4 */ u32 switch1;
    /* 0x8 */ u32 chest;
    /* 0xC */ u32 collectible;
} PersistentCycleSceneFlags; // size = 0x10

#define PERSISTENT_CYCLE_FLAGS_SET(switch0, switch1, chest, collectible) { switch0, switch1, chest, collectible },
#define PERSISTENT_CYCLE_FLAGS_NONE PERSISTENT_CYCLE_FLAGS_SET(0, 0, 0, 0)

#define DEFINE_SCENE(_name, _enumValue, _textId, _drawConfig, _restrictionFlags, persistentCycleFlags) \
    persistentCycleFlags
#define DEFINE_SCENE_UNSET(_enumValue) PERSISTENT_CYCLE_FLAGS_NONE

/**
 * Array of bitwise flags which won't be turned off on a cycle reset (will persist between cycles)
 */
PersistentCycleSceneFlags sPersistentCycleSceneFlags[SCENE_MAX] = {
#include "tables/scene_table.h"
};

#undef DEFINE_SCENE
#undef DEFINE_SCENE_UNSET

// Each flag has 2 bits to store persistence over the three-day reset cycle
// Only 1 of these bits need to be set to persist (Values 1, 2, 3).
// Therefore, the final game does not distinguish between these two macros in use
#define PERSISTENT_WEEKEVENTREG(flag) (3 << (2 * BIT_FLAG_TO_SHIFT(flag)))
#define PERSISTENT_WEEKEVENTREG_ALT(flag) (2 << (2 * BIT_FLAG_TO_SHIFT(flag)))

// weekEventReg flags which will be not be cleared on a cycle reset
//! @note The index of the flag in this array must be the same to its index in the WeekeventReg array
//!       Only the mask is read from the `PERSISTENT_` macros.
u16 sPersistentCycleWeekEventRegs[ARRAY_COUNT(gMmSave.saveInfo.weekEventReg)] = {
    /*  0 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_TERMINA_FIELD) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_IKANA_GRAVEYARD) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_ROMANI_RANCH) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_GORMAN_TRACK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_MOUNTAIN_VILLAGE_WINTER) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_GORON_SHRINE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_SNOWHEAD),
    /*  1 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_SOUTHERN_SWAMP_POISONED) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WOODFALL) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_DEKU_PALACE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_GREAT_BAY_COAST) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_PIRATES_FORTRESS) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_ZORA_HALL) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WATERFALL_RAPIDS) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_IKANA_CANYON),
    /*  2 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_IKANA_CASTLE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_STONE_TOWER) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_STONE_TOWER_INVERTED) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_EAST_CLOCK_TOWN) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WEST_CLOCK_TOWN) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_NORTH_CLOCK_TOWN) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WOODFALL_TEMPLE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_SNOWHEAD_TEMPLE),
    /*  3 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_PIRATES_FORTRESS_EXTERIOR) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_STONE_TOWER_TEMPLE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_STONE_TOWER_TEMPLE_INVERTED) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_THE_MOON) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_MOON_DEKU_TRIAL) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_MOON_GORON_TRIAL) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_MOON_ZORA_TRIAL) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_03_80),
    /*  4 */ 0,
    /*  5 */ 0,
    /*  6 */ 0,
    /*  7 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WOODFALL_TEMPLE_PRISON),
    /*  8 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_DOGGY_RACETRACK_HEART_PIECE),
    /*  9 */ 0,
    /* 10 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_BANK_WALLET_UPGRADE),
    /* 11 */ 0,
    /* 12 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE),
    /* 13 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_OCEANSIDE_WALLET_UPGRADE),
    /* 14 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_DEKU_PLAYGROUND_HEART_PIECE),
    /* 15 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_15_20),
    /* 16 */ 0,
    /* 17 */ 0,
    /* 18 */ 0,
    /* 19 */ 0,
    /* 20 */ 0,
    /* 21 */ 0,
    /* 22 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_ALIENS_BOTTLE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_HONEY_AND_DARLING_HEART_PIECE),
    /* 23 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_GREAT_SPIN_ATTACK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_BEAVER_RACE_BOTTLE),
    /* 24 */ PERSISTENT_WEEKEVENTREG_ALT(WEEKEVENTREG_24_02) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_24_80),
    /* 25 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_BEAVER_BROS_HEART_PIECE),
    /* 26 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_26_40),
    /* 27 */ 0,
    /* 28 */ 0,
    /* 29 */ 0,
    /* 30 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_30_10) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_30_20) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_30_40) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_30_80),
    /* 31 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_31_01) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_31_02) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_31_04) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_31_20),
    /* 32 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SEAHORSE_HEART_PIECE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_HEART_PIECE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_TOWN_SHOOTING_GALLERY_HEART_PIECE),
    /* 33 */ 0,
    /* 34 */ 0,
    /* 35 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_TINGLE_MAP_BOUGHT_CLOCK_TOWN) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_TINGLE_MAP_BOUGHT_WOODFALL) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_TINGLE_MAP_BOUGHT_SNOWHEAD) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_TINGLE_MAP_BOUGHT_ROMANI_RANCH) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_TINGLE_MAP_BOUGHT_GREAT_BAY) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_TINGLE_MAP_BOUGHT_STONE_TOWER) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_FROG_CHOIR_HEART_PIECE),
    /* 36 */ 0,
    /* 37 */ 0,
    /* 38 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_38_20),
    /* 39 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_EVAN_HEART_PIECE),
    /* 40 */ 0,
    /* 41 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_GORON_RACE_BOTTLE),
    /* 42 */ 0,
    /* 43 */ 0,
    /* 44 */ 0,
    /* 45 */ 0,
    /* 46 */ 0,
    /* 47 */ 0,
    /* 48 */ 0,
    /* 49 */ 0,
    /* 50 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_50_02) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_50_04),
    /* 51 */ PERSISTENT_WEEKEVENTREG_ALT(WEEKEVENTREG_51_04),
    /* 52 */ 0,
    /* 53 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_BUSINESS_SCRUB_HEART_PIECE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_53_40),
    /* 54 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SPIRIT_HOUSE_HEART_PIECE),
    /* 55 */ 0,
    /* 56 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_MARINE_RESEARCH_LAB_FISH_HEART_PIECE),
    /* 57 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_57_08),
    /* 58 */ 0,
    /* 59 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_59_04) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_BANK_HEART_PIECE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SWAMP_SHOOTING_GALLERY_QUIVER_UPGRADE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_TOWN_SHOOTING_GALLERY_QUIVER_UPGRADE),
    /* 60 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_MAYOR_HEART_PIECE),
    /* 61 */ 0,
    /* 62 */ 0,
    /* 63 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_SWORDSMANS_SCHOOL_HEART_PIECE),
    /* 64 */ 0,
    /* 65 */ 0,
    /* 66 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_ANJU) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_KAFEI) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_CURIOSITY_SHOP_MAN) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_BOMB_SHOP_LADY) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_ROMANI) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_CREMIA) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_MAYOR_DOTOUR) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_MADAME_AROMA),
    /* 67 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_TOTO) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_GORMAN) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_POSTMAN) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_ROSA_SISTERS) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_TOILET_HAND) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_ANJUS_GRANDMOTHER) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_KAMARO) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_GROG),
    /* 68 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_SHIRO) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_GURU_GURU) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_MET_BOMBERS) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_ROOM_KEY) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_PROMISED_MIDNIGHT_MEETING) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_PROMISED_TO_MEET_KAFEI) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_LETTER_TO_KAFEI),
    /* 69 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_DEPOSITED_LETTER_TO_KAFEI) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_PENDANT_OF_MEMORIES) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_DELIVERED_PENDANT_OF_MEMORIES) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_ESCAPED_SAKONS_HIDEOUT) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_PROMISED_TO_HELP_WITH_ALIENS) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_DEFENDED_AGAINST_ALIENS) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_ALIENS_BOTTLE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_ESCORTED_CREMIA),
    /* 70 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_ROMANIS_MASK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_KEATON_MASK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_PRIORITY_MAIL) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_DELIVERED_PRIORITY_MAIL) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_LEARNED_SECRET_CODE) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_BOMBERS_NOTEBOOK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_MAYOR_HP) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_ROSA_SISTERS_HP),
    /* 71 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_TOILET_HAND_HP) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_GRANDMA_SHORT_STORY_HP) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_GRANDMA_LONG_STORY_HP) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_POSTMAN_HP) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_KAFEIS_MASK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_ALL_NIGHT_MASK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_BUNNY_HOOD) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_GAROS_MASK),
    /* 72 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_CIRCUS_LEADERS_MASK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_POSTMANS_HAT) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_COUPLES_MASK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_BLAST_MASK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_KAMAROS_MASK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_STONE_MASK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_BOMBERS_NOTEBOOK_EVENT_RECEIVED_BREMEN_MASK) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_72_80),
    /* 73 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_73_08),
    /* 74 */ 0,
    /* 75 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_ROSA_SISTERS_HEART_PIECE),
    /* 76 */ 0,
    /* 77 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_POSTMAN_COUNTING_GAME_HEART_PIECE),
    /* 78 */ 0,
    /* 79 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KEATON_HEART_PIECE),
    /* 80 */ 0,
    /* 81 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_81_08),
    /* 82 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_FISHERMANS_JUMPING_GAME_HEART_PIECE),
    /* 83 */ 0,
    /* 84 */ 0,
    /* 85 */ 0,
    /* 86 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_86_80),
    /* 87 */
    PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_87_04) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_87_08) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_87_10) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_87_20) |
        PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_87_40) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_87_80),
    /* 88 */ 0,
    /* 89 */ 0,
    /* 90 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_GOSSIP_STONE_GROTTO_HEART_PIECE),
    /* 91 */ 0,
    /* 92 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_92_80),
    /* 93 */ PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_93_04) | PERSISTENT_WEEKEVENTREG(WEEKEVENTREG_93_08),
    /* 94 */ 0,
    /* 95 */ 0,
    /* 96 */ 0,
    /* 97 */ 0,
    /* 98 */ 0,
    /* 99 */ 0,
};

u8 gAmmoItems[ITEM_NUM_SLOTS] = {
    ITEM_NONE,           // SLOT_OCARINA
    ITEM_BOW,            // SLOT_BOW
    ITEM_NONE,           // SLOT_ARROW_FIRE
    ITEM_NONE,           // SLOT_ARROW_ICE
    ITEM_NONE,           // SLOT_ARROW_LIGHT
    ITEM_NONE,           // SLOT_TRADE_DEED
    ITEM_BOMB,           // SLOT_BOMB
    ITEM_BOMBCHU,        // SLOT_BOMBCHU
    ITEM_DEKU_STICK,     // SLOT_DEKU_STICK
    ITEM_DEKU_NUT,       // SLOT_DEKU_NUT
    ITEM_MAGIC_BEANS,    // SLOT_MAGIC_BEANS
    ITEM_NONE,           // SLOT_TRADE_KEY_MAMA
    ITEM_POWDER_KEG,     // SLOT_POWDER_KEG
    ITEM_PICTOGRAPH_BOX, // SLOT_PICTOGRAPH_BOX
    ITEM_NONE,           // SLOT_LENS_OF_TRUTH
    ITEM_NONE,           // SLOT_HOOKSHOT
    ITEM_NONE,           // SLOT_SWORD_GREAT_FAIRY
    ITEM_NONE,           // SLOT_TRADE_COUPLE
    ITEM_NONE,           // SLOT_BOTTLE_1
    ITEM_NONE,           // SLOT_BOTTLE_2
    ITEM_NONE,           // SLOT_BOTTLE_3
    ITEM_NONE,           // SLOT_BOTTLE_4
    ITEM_NONE,           // SLOT_BOTTLE_5
    ITEM_NONE,           // SLOT_BOTTLE_6
};

// Bit Flag array in which sBitFlags8[n] is (1 << n)
u8 sBitFlags8[] = {
    (1 << 0), (1 << 1), (1 << 2), (1 << 3), (1 << 4), (1 << 5), (1 << 6), (1 << 7),
};

void Sram_ActivateOwl(u8 owlWarpId) {
    SET_OWL_STATUE_ACTIVATED(owlWarpId);

    if (gMmSave.saveInfo.playerData.owlWarpId == OWL_WARP_NONE) {
        gMmSave.saveInfo.playerData.owlWarpId = owlWarpId;
    }
}

void Sram_ClearHighscores(void) {
    HS_SET_BOAT_ARCHERY_HIGH_SCORE(19);
    HS_SET_HIGH_SCORE_3_LOWER(10);
    HS_SET_HORSE_BACK_BALLOON_TIME(SECONDS_TO_TIMER(60));
    HS_SET_TOWN_SHOOTING_GALLERY_HIGH_SCORE(39);
    HS_SET_SWAMP_SHOOTING_GALLERY_HIGH_SCORE(10);

    gMmSave.saveInfo.dekuPlaygroundHighScores[0] = SECONDS_TO_TIMER(75);
    gMmSave.saveInfo.dekuPlaygroundHighScores[1] = SECONDS_TO_TIMER(75);
    gMmSave.saveInfo.dekuPlaygroundHighScores[2] = SECONDS_TO_TIMER(76);
}

/**
 * Clears specific weekEventReg flags. Used by the "Dawn of the First Day" message
 */
void Sram_ClearFlagsAtDawnOfTheFirstDay(void) {
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_55_02);
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_90_01);
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_89_40);
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_89_08);
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_85_80);
}

/**
 * Used by Song of Time (when clicking "Yes") and (indirectly) by the "Dawn of the New Day" cutscene
 */
void Sram_SaveEndOfCycle(PlayState* play) {
    s16 sceneId;
    s32 j;
    s32 i;
    u8 slot;
    u8 item;

    gMmSave.timeSpeedOffset = 0;
    gMmSave.eventDayCount = 0;
    gMmSave.day = 0;
    gMmSave.time = CLOCK_TIME(6, 0) - 1;

    gMmSave.saveInfo.playerData.threeDayResetCount++;
    if (gMmSave.saveInfo.playerData.threeDayResetCount > 999) {
        gMmSave.saveInfo.playerData.threeDayResetCount = 999;
    }

    sceneId = Play_GetOriginalSceneId(play->sceneId);
    Play_SaveCycleSceneFlags(play);

    play->actorCtx.sceneFlags.chest &= sPersistentCycleSceneFlags[sceneId].chest;
    play->actorCtx.sceneFlags.switches[0] &= sPersistentCycleSceneFlags[sceneId].switch0;
    play->actorCtx.sceneFlags.switches[1] &= sPersistentCycleSceneFlags[sceneId].switch1;
    play->actorCtx.sceneFlags.collectible[0] &= sPersistentCycleSceneFlags[sceneId].collectible;
    play->actorCtx.sceneFlags.clearedRoom = 0;

    for (i = 0; i < SCENE_MAX; i++) {
        gSaveContext.cycleSceneFlags[i].switch0 =
            ((void)0, gSaveContext.cycleSceneFlags[i].switch0) & sPersistentCycleSceneFlags[i].switch0;
        gSaveContext.cycleSceneFlags[i].switch1 =
            ((void)0, gSaveContext.cycleSceneFlags[i].switch1) & sPersistentCycleSceneFlags[i].switch1;
        gSaveContext.cycleSceneFlags[i].chest =
            ((void)0, gSaveContext.cycleSceneFlags[i].chest) & sPersistentCycleSceneFlags[i].chest;
        gSaveContext.cycleSceneFlags[i].collectible =
            ((void)0, gSaveContext.cycleSceneFlags[i].collectible) & sPersistentCycleSceneFlags[i].collectible;
        gSaveContext.cycleSceneFlags[i].clearedRoom = 0;
        gMmSave.saveInfo.permanentSceneFlags[i].unk_14 = 0;
        gMmSave.saveInfo.permanentSceneFlags[i].rooms = 0;
    }

    for (; i < ARRAY_COUNT(gSaveContext.cycleSceneFlags); i++) {
        gSaveContext.cycleSceneFlags[i].chest = 0;
        gSaveContext.cycleSceneFlags[i].switch0 = 0;
        gSaveContext.cycleSceneFlags[i].switch1 = 0;
        gSaveContext.cycleSceneFlags[i].clearedRoom = 0;
        gSaveContext.cycleSceneFlags[i].collectible = 0;
    }

    for (i = 0; i < ARRAY_COUNT(gSaveContext.masksGivenOnMoon); i++) {
        gSaveContext.masksGivenOnMoon[i] = 0;
    }

    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_84_20)) {
        Inventory_DeleteItem(ITEM_MASK_FIERCE_DEITY, SLOT(ITEM_MASK_FIERCE_DEITY));
    }

    for (i = 0; i < ARRAY_COUNT(sPersistentCycleWeekEventRegs); i++) {
        u16 isPersistentBits = sPersistentCycleWeekEventRegs[i];

        for (j = 0; j < ARRAY_COUNT(sBitFlags8); j++) {
            if (!(isPersistentBits & 3)) {
                gMmSave.saveInfo.weekEventReg[i] =
                    ((void)0, gMmSave.saveInfo.weekEventReg[i]) & (0xFF ^ sBitFlags8[j]);
            }
            isPersistentBits >>= 2;
        }
    }

    for (i = 0; i < ARRAY_COUNT(gSaveContext.eventInf); i++) {
        gSaveContext.eventInf[i] = 0;
    }

    CLEAR_EVENTINF(EVENTINF_THREEDAYRESET_LOST_RUPEES);
    CLEAR_EVENTINF(EVENTINF_THREEDAYRESET_LOST_BOMB_AMMO);
    CLEAR_EVENTINF(EVENTINF_THREEDAYRESET_LOST_NUT_AMMO);
    CLEAR_EVENTINF(EVENTINF_THREEDAYRESET_LOST_STICK_AMMO);
    CLEAR_EVENTINF(EVENTINF_THREEDAYRESET_LOST_ARROW_AMMO);

    if (gMmSave.saveInfo.playerData.rupees != 0) {
        SET_EVENTINF(EVENTINF_THREEDAYRESET_LOST_RUPEES);
    }

    if (INV_CONTENT(ITEM_BOMB) == ITEM_BOMB) {
        item = INV_CONTENT(ITEM_BOMB);
        if (AMMO(item) != 0) {
            SET_EVENTINF(EVENTINF_THREEDAYRESET_LOST_BOMB_AMMO);
        }
    }
    if (INV_CONTENT(ITEM_DEKU_NUT) == ITEM_DEKU_NUT) {
        item = INV_CONTENT(ITEM_DEKU_NUT);
        if (AMMO(item) != 0) {
            SET_EVENTINF(EVENTINF_THREEDAYRESET_LOST_NUT_AMMO);
        }
    }
    if (INV_CONTENT(ITEM_DEKU_STICK) == ITEM_DEKU_STICK) {
        item = INV_CONTENT(ITEM_DEKU_STICK);
        if (AMMO(item) != 0) {
            SET_EVENTINF(EVENTINF_THREEDAYRESET_LOST_STICK_AMMO);
        }
    }
    if (INV_CONTENT(ITEM_BOW) == ITEM_BOW) {
        item = INV_CONTENT(ITEM_BOW);
        if (AMMO(item) != 0) {
            SET_EVENTINF(EVENTINF_THREEDAYRESET_LOST_ARROW_AMMO);
        }
    }

    for (i = 0; i < ITEM_NUM_SLOTS; i++) {
        if (gAmmoItems[i] != ITEM_NONE) {
            if ((gMmSave.saveInfo.inventory.items[i] != ITEM_NONE) && (i != SLOT_PICTOGRAPH_BOX)) {
                item = gMmSave.saveInfo.inventory.items[i];
                AMMO(item) = 0;
            }
        }
    }

    for (i = SLOT_BOTTLE_1; i <= SLOT_BOTTLE_6; i++) {
        // Check for all bottled items
        if (gMmSave.saveInfo.inventory.items[i] >= ITEM_POTION_RED) {
            if (gMmSave.saveInfo.inventory.items[i] <= ITEM_OBABA_DRINK) {
                for (j = EQUIP_SLOT_C_LEFT; j <= EQUIP_SLOT_C_RIGHT; j++) {
                    if (GET_CUR_FORM_BTN_ITEM(j) == gMmSave.saveInfo.inventory.items[i]) {
                        SET_CUR_FORM_BTN_ITEM(j, ITEM_BOTTLE);
                        Interface_LoadItemIconImpl(play, j);
                    }
                }
                gMmSave.saveInfo.inventory.items[i] = ITEM_BOTTLE;
            }
        }
    }

    REMOVE_QUEST_ITEM(QUEST_PICTOGRAPH);

    if (gMmSave.saveInfo.playerData.health < 0x30) {
        gMmSave.saveInfo.playerData.health = 0x30;
    }

    if (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD) <= EQUIP_VALUE_SWORD_RAZOR) {
        SET_EQUIP_VALUE(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_KOKIRI);

        if (CUR_FORM == 0) {
            if ((STOLEN_ITEM_1 >= ITEM_SWORD_GILDED) || (STOLEN_ITEM_2 >= ITEM_SWORD_GILDED)) {
                CUR_FORM_EQUIP(EQUIP_SLOT_B) = ITEM_SWORD_GILDED;
                SET_EQUIP_VALUE(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_GILDED);
            } else {
                CUR_FORM_EQUIP(EQUIP_SLOT_B) = ITEM_SWORD_KOKIRI;
            }
        } else {
            if ((STOLEN_ITEM_1 >= ITEM_SWORD_GILDED) || (STOLEN_ITEM_2 >= ITEM_SWORD_GILDED)) {
                BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_B) = ITEM_SWORD_GILDED;
                SET_EQUIP_VALUE(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_GILDED);
            } else {
                BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_B) = ITEM_SWORD_KOKIRI;
            }
        }
    }

    if ((STOLEN_ITEM_1 == ITEM_SWORD_GREAT_FAIRY) || (STOLEN_ITEM_2 == ITEM_SWORD_GREAT_FAIRY)) {
        INV_CONTENT(ITEM_SWORD_GREAT_FAIRY) = ITEM_SWORD_GREAT_FAIRY;
    }

    if (STOLEN_ITEM_1 == ITEM_BOTTLE) {
        slot = SLOT(ITEM_BOTTLE);
        for (i = BOTTLE_FIRST; i < BOTTLE_MAX; i++) {
            if (gMmSave.saveInfo.inventory.items[slot + i] == ITEM_NONE) {
                gMmSave.saveInfo.inventory.items[slot + i] = ITEM_BOTTLE;
                break;
            }
        }
    }

    if (STOLEN_ITEM_2 == ITEM_BOTTLE) {
        slot = SLOT(ITEM_BOTTLE);
        for (i = BOTTLE_FIRST; i < BOTTLE_MAX; i++) {
            if (gMmSave.saveInfo.inventory.items[slot + i] == ITEM_NONE) {
                gMmSave.saveInfo.inventory.items[slot + i] = ITEM_BOTTLE;
                break;
            }
        }
    }

    SET_STOLEN_ITEM_1(STOLEN_ITEM_NONE);
    SET_STOLEN_ITEM_2(STOLEN_ITEM_NONE);

    Inventory_DeleteItem(ITEM_OCARINA_FAIRY, SLOT_TRADE_DEED);
    Inventory_DeleteItem(ITEM_SLINGSHOT, SLOT_TRADE_KEY_MAMA);
    Inventory_DeleteItem(ITEM_LONGSHOT, SLOT_TRADE_COUPLE);

    for (j = EQUIP_SLOT_C_LEFT; j <= EQUIP_SLOT_C_RIGHT; j++) {
        if ((GET_CUR_FORM_BTN_ITEM(j) >= ITEM_MOONS_TEAR) && (GET_CUR_FORM_BTN_ITEM(j) <= ITEM_PENDANT_OF_MEMORIES)) {
            SET_CUR_FORM_BTN_ITEM(j, ITEM_NONE);
            Interface_LoadItemIconImpl(play, j);
        }
    }

    gMmSave.saveInfo.skullTokenCount &= ~0xFFFF0000;
    gMmSave.saveInfo.skullTokenCount &= ~0x0000FFFF;
    gMmSave.saveInfo.unk_EA0 = 0;

    gMmSave.saveInfo.alienInfo[0] = 0;
    gMmSave.saveInfo.alienInfo[1] = 0;
    gMmSave.saveInfo.alienInfo[2] = 0;
    gMmSave.saveInfo.alienInfo[3] = 0;
    gMmSave.saveInfo.alienInfo[4] = 0;
    gMmSave.saveInfo.alienInfo[5] = 0;
    gMmSave.saveInfo.alienInfo[6] = 0;

    Sram_ClearHighscores();

    for (i = 0; i < 8; i++) {
        gMmSave.saveInfo.inventory.dungeonItems[i] &= (u8)~1; // remove boss key
        DUNGEON_KEY_COUNT(i) = 0;
        gMmSave.saveInfo.inventory.strayFairies[i] = 0;
    }

    gMmSave.saveInfo.playerData.rupees = 0;
    gMmSave.saveInfo.scarecrowSpawnSongSet = false;
    gSaveContext.powderKegTimer = 0;
    gSaveContext.unk_1014 = 0;
    gSaveContext.jinxTimer = 0;
    gSaveContext.rupeeAccumulator = 0;

    Horse_ResetHorseData(play);
}

void Sram_IncrementDay(void) {
    if (CURRENT_DAY <= 3) {
        gMmSave.day++;
        gMmSave.eventDayCount++;
    }

    gMmSave.saveInfo.bombersCaughtNum = 0;
    gMmSave.saveInfo.bombersCaughtOrder[0] = 0;
    gMmSave.saveInfo.bombersCaughtOrder[1] = 0;
    gMmSave.saveInfo.bombersCaughtOrder[2] = 0;
    gMmSave.saveInfo.bombersCaughtOrder[3] = 0;
    gMmSave.saveInfo.bombersCaughtOrder[4] = 0;

    CLEAR_WEEKEVENTREG(WEEKEVENTREG_73_10);
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_85_02);
}

void Sram_ResetSaveFromMoonCrash(void) {
    s32 i;
    s32 cutsceneIndex = gMmSave.cutsceneIndex;

    SaveRaw_Read();
    gMmSave.cutsceneIndex = cutsceneIndex;

    for (i = 0; i < ARRAY_COUNT(gSaveContext.eventInf); i++) {
        gSaveContext.eventInf[i] = 0;
    }

    for (i = 0; i < ARRAY_COUNT(gSaveContext.cycleSceneFlags); i++) {
        gSaveContext.cycleSceneFlags[i].chest = gMmSave.saveInfo.permanentSceneFlags[i].chest;
        gSaveContext.cycleSceneFlags[i].switch0 = gMmSave.saveInfo.permanentSceneFlags[i].switch0;
        gSaveContext.cycleSceneFlags[i].switch1 = gMmSave.saveInfo.permanentSceneFlags[i].switch1;
        gSaveContext.cycleSceneFlags[i].clearedRoom = gMmSave.saveInfo.permanentSceneFlags[i].clearedRoom;
        gSaveContext.cycleSceneFlags[i].collectible = gMmSave.saveInfo.permanentSceneFlags[i].collectible;
    }

    for (i = 0; i < TIMER_ID_MAX; i++) {
        gSaveContext.timerStates[i] = TIMER_STATE_OFF;
        gSaveContext.timerCurTimes[i] = SECONDS_TO_TIMER(0);
        gSaveContext.timerTimeLimits[i] = SECONDS_TO_TIMER(0);
        gSaveContext.timerStartOsTimes[i] = 0;
        gSaveContext.timerStopTimes[i] = SECONDS_TO_TIMER(0);
        gSaveContext.timerPausedOsTimes[i] = 0;
    }

    D_801BDAA0 = true;
    gHorseIsMounted = false;
    gSaveContext.powderKegTimer = 0;
    gSaveContext.unk_1014 = 0;
    gSaveContext.jinxTimer = 0;
}

void Sram_UpdatePermanentFlags(void) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(gSaveContext.cycleSceneFlags); i++) {
        gMmSave.saveInfo.permanentSceneFlags[i].chest = gSaveContext.cycleSceneFlags[i].chest;
        gMmSave.saveInfo.permanentSceneFlags[i].switch0 = gSaveContext.cycleSceneFlags[i].switch0;
        gMmSave.saveInfo.permanentSceneFlags[i].switch1 = gSaveContext.cycleSceneFlags[i].switch1;
        gMmSave.saveInfo.permanentSceneFlags[i].clearedRoom = gSaveContext.cycleSceneFlags[i].clearedRoom;
        gMmSave.saveInfo.permanentSceneFlags[i].collectible = gSaveContext.cycleSceneFlags[i].collectible;
    }
}

/**
 * Saves when beating the game, after showing the "Dawn of the New Day" message
 */
void Sram_SaveSpecialNewDay(PlayState* play) {
    s32 cutsceneIndex = gMmSave.cutsceneIndex;
    s32 day;
    u16 time = CURRENT_TIME;

    day = gMmSave.day;

    CLEAR_WEEKEVENTREG(WEEKEVENTREG_84_20);

    Sram_SaveEndOfCycle(play);
    Sram_UpdatePermanentFlags();

    gMmSave.day = day;
    gMmSave.time = time;
    gMmSave.cutsceneIndex = cutsceneIndex;
    SaveRaw_Write();
}
