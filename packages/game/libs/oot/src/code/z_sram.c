#include "global.h"
#include "terminal.h"
#include "versions.h"

static OotSavePlayerData sDebugSavePlayerData = {
    { 'Z', 'E', 'L', 'D', 'A', 'Z' }, // newf
    0,                                // deaths
#if OOT_VERSION < PAL_1_0
    {
        0x81, // リ
        0x87, // ン
        0x61, // ク
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
    }, // playerName
#else
    {
        FILENAME_UPPERCASE('L'),
        FILENAME_UPPERCASE('I'),
        FILENAME_UPPERCASE('N'),
        FILENAME_UPPERCASE('K'),
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
    }, // playerName
#endif
    0,                  // n64ddFlag
    0xE0,               // healthCapacity
    0xE0,               // health
    0,                  // magicLevel
    MAGIC_NORMAL_METER, // magic
    150,                // rupees
    8,                  // swordHealth
    0,                  // naviTimer
    true,               // isMagicAcquired
    0,                  // unk_1F
    false,              // isDoubleMagicAcquired
    false,              // isDoubleDefenseAcquired
    0,                  // bgsFlag
    0,                  // ocarinaGameRoundNum
    {
        { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
        { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
        0,                                              // equipment
    },                                                  // childEquips
    {
        { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
        { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
        0,                                              // equipment
    },                                                  // adultEquips
    0,                                                  // unk_38
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },       // unk_3C
    SCENE_HYRULE_FIELD,                                 // savedSceneId
};

static OotItemEquips sDebugSaveEquips = {
    { ITEM_SWORD_MASTER, ITEM_BOW, ITEM_BOMB, ITEM_OCARINA_FAIRY }, // buttonItems
    { SLOT_OOT_BOW, SLOT_OOT_BOMB, SLOT_OOT_OCARINA },              // cButtonSlots
    // equipment
    (EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4)) | (EQUIP_VALUE_SHIELD_HYLIAN << (EQUIP_TYPE_SHIELD * 4)) |
        (EQUIP_VALUE_TUNIC_KOKIRI << (EQUIP_TYPE_TUNIC * 4)) | (EQUIP_VALUE_BOOTS_KOKIRI << (EQUIP_TYPE_BOOTS * 4)),
};

static OotInventory sDebugSaveInventory = {
    // items
    {
        ITEM_DEKU_STICK,          // SLOT_OOT_DEKU_STICK
        ITEM_DEKU_NUT,            // SLOT_OOT_DEKU_NUT
        ITEM_BOMB,                // SLOT_OOT_BOMB
        ITEM_BOW,                 // SLOT_OOT_BOW
        ITEM_ARROW_FIRE,          // SLOT_OOT_ARROW_FIRE
        ITEM_DINS_FIRE,           // SLOT_OOT_DINS_FIRE
        ITEM_SLINGSHOT,           // SLOT_OOT_SLINGSHOT
        ITEM_OCARINA_FAIRY,       // SLOT_OOT_OCARINA
        ITEM_BOMBCHU,             // SLOT_OOT_BOMBCHU
        ITEM_HOOKSHOT,            // SLOT_OOT_HOOKSHOT
        ITEM_ARROW_ICE,           // SLOT_OOT_ARROW_ICE
        ITEM_FARORES_WIND,        // SLOT_OOT_FARORES_WIND
        ITEM_BOOMERANG,           // SLOT_OOT_BOOMERANG
        ITEM_LENS_OF_TRUTH,       // SLOT_OOT_LENS_OF_TRUTH
        ITEM_MAGIC_BEAN,          // SLOT_OOT_MAGIC_BEAN
        ITEM_HAMMER,              // SLOT_OOT_HAMMER
        ITEM_ARROW_LIGHT,         // SLOT_OOT_ARROW_LIGHT
        ITEM_NAYRUS_LOVE,         // SLOT_OOT_NAYRUS_LOVE
        ITEM_BOTTLE_EMPTY,        // SLOT_OOT_BOTTLE_1
        ITEM_BOTTLE_POTION_RED,   // SLOT_OOT_BOTTLE_2
        ITEM_BOTTLE_POTION_GREEN, // SLOT_OOT_BOTTLE_3
        ITEM_BOTTLE_POTION_BLUE,  // SLOT_OOT_BOTTLE_4
        ITEM_POCKET_EGG,          // SLOT_OOT_TRADE_ADULT
        ITEM_WEIRD_EGG,           // SLOT_OOT_TRADE_CHILD
    },
    // ammo
    {
        50, // SLOT_OOT_DEKU_STICK
        50, // SLOT_OOT_DEKU_NUT
        10, // SLOT_OOT_BOMB
        30, // SLOT_OOT_BOW
        1,  // SLOT_OOT_ARROW_FIRE
        1,  // SLOT_OOT_DINS_FIRE
        30, // SLOT_OOT_SLINGSHOT
        1,  // SLOT_OOT_OCARINA
        50, // SLOT_OOT_BOMBCHU
        1,  // SLOT_OOT_HOOKSHOT
        1,  // SLOT_OOT_ARROW_ICE
        1,  // SLOT_OOT_FARORES_WIND
        1,  // SLOT_OOT_BOOMERANG
        1,  // SLOT_OOT_LENS_OF_TRUTH
        1,  // SLOT_OOT_MAGIC_BEAN
        1   // SLOT_OOT_HAMMER
    },
    // equipment
    ((((1 << EQUIP_INV_SWORD_KOKIRI) << (EQUIP_TYPE_SWORD * 4)) |
      ((1 << EQUIP_INV_SWORD_MASTER) << (EQUIP_TYPE_SWORD * 4)) |
      ((1 << EQUIP_INV_SWORD_BIGGORON) << (EQUIP_TYPE_SWORD * 4))) |
     (((1 << EQUIP_INV_SHIELD_DEKU) << (EQUIP_TYPE_SHIELD * 4)) |
      ((1 << EQUIP_INV_SHIELD_HYLIAN) << (EQUIP_TYPE_SHIELD * 4)) |
      ((1 << EQUIP_INV_SHIELD_MIRROR) << (EQUIP_TYPE_SHIELD * 4))) |
     (((1 << EQUIP_INV_TUNIC_KOKIRI) << (EQUIP_TYPE_TUNIC * 4)) |
      ((1 << EQUIP_INV_TUNIC_GORON) << (EQUIP_TYPE_TUNIC * 4)) |
      ((1 << EQUIP_INV_TUNIC_ZORA) << (EQUIP_TYPE_TUNIC * 4))) |
     (((1 << EQUIP_INV_BOOTS_KOKIRI) << (EQUIP_TYPE_BOOTS * 4)) |
      ((1 << EQUIP_INV_BOOTS_IRON) << (EQUIP_TYPE_BOOTS * 4)) |
      ((1 << EQUIP_INV_BOOTS_HOVER) << (EQUIP_TYPE_BOOTS * 4)))),
    0x125249,                                                       // upgrades
    0x1E3FFFF,                                                      // questItems
    { 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // dungeonItems
    { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // dungeonKeys
    0,                                                              // defenseHearts
    0,                                                              // gsTokens
};

static OotChecksum sDebugSaveChecksum = { 0 };

/**
 *  Initialize debug save. This is also used on the Title Screen
 *  This save has a mostly full inventory with 10 hearts and single magic.
 *
 *  Some noteable flags that are set:
 *  Showed Mido sword/shield, met Deku Tree, Deku Tree mouth opened,
 *  used blue warp in Gohmas room, Zelda fled castle, light arrow cutscene watched,
 *  and set water level in Water Temple to lowest level.
 */
void Sram_InitDebugSave(void) {
    bzero(&gOotSave.info, sizeof(OotSaveInfo));
    gOotSave.totalDays = 0;
    gOotSave.bgsDayCount = 0;

    gOotSave.info.playerData = sDebugSavePlayerData;
    gOotSave.info.equips = sDebugSaveEquips;
    gOotSave.info.inventory = sDebugSaveInventory;

    gOotSave.info.checksum = sDebugSaveChecksum;
    gOotSave.info.horseData.sceneId = SCENE_HYRULE_FIELD;
    gOotSave.info.horseData.pos.x = -1840;
    gOotSave.info.horseData.pos.y = 72;
    gOotSave.info.horseData.pos.z = 5497;
    gOotSave.info.horseData.angle = -0x6AD9;
    gOotSave.info.infTable[0] |= 0x5009;
    gOotSave.info.eventChkInf[0] |= 0x123F;
    SET_EVENTCHKINF(EVENTCHKINF_80);
    SET_EVENTCHKINF(EVENTCHKINF_C4);

    if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
        gOotSave.info.equips.buttonItems[0] = ITEM_SWORD_KOKIRI;
        Inventory_ChangeEquipment(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_KOKIRI);
        if (gSaveFileNum == -1) {
            gOotSave.info.equips.buttonItems[1] = ITEM_SLINGSHOT;
            gOotSave.info.equips.cButtonSlots[0] = SLOT_OOT_SLINGSHOT;
            Inventory_ChangeEquipment(EQUIP_TYPE_SHIELD, EQUIP_VALUE_SHIELD_DEKU);
        }
    }

    gOotSave.entranceIndex = ENTR_HYRULE_FIELD_0;
    gOotSave.info.playerData.magicLevel = 0;
    gOotSave.info.sceneFlags[SCENE_WATER_TEMPLE].swch = 0x40000000;
}

static s16 sDungeonEntrances[] = {
    ENTR_DEKU_TREE_0,                      // SCENE_DEKU_TREE
    ENTR_DODONGOS_CAVERN_0,                // SCENE_DODONGOS_CAVERN
    ENTR_JABU_JABU_0,                      // SCENE_JABU_JABU
    ENTR_FOREST_TEMPLE_0,                  // SCENE_FOREST_TEMPLE
    ENTR_FIRE_TEMPLE_0,                    // SCENE_FIRE_TEMPLE
    ENTR_WATER_TEMPLE_0,                   // SCENE_WATER_TEMPLE
    ENTR_SPIRIT_TEMPLE_0,                  // SCENE_SPIRIT_TEMPLE
    ENTR_SHADOW_TEMPLE_0,                  // SCENE_SHADOW_TEMPLE
    ENTR_BOTTOM_OF_THE_WELL_0,             // SCENE_BOTTOM_OF_THE_WELL
    ENTR_ICE_CAVERN_0,                     // SCENE_ICE_CAVERN
    ENTR_GANONS_TOWER_0,                   // SCENE_GANONS_TOWER
    ENTR_GERUDO_TRAINING_GROUND_0,         // SCENE_GERUDO_TRAINING_GROUND
    ENTR_THIEVES_HIDEOUT_0,                // SCENE_THIEVES_HIDEOUT
    ENTR_INSIDE_GANONS_CASTLE_0,           // SCENE_INSIDE_GANONS_CASTLE
    ENTR_GANONS_TOWER_COLLAPSE_INTERIOR_0, // SCENE_GANONS_TOWER_COLLAPSE_INTERIOR
    ENTR_INSIDE_GANONS_CASTLE_COLLAPSE_0,  // SCENE_INSIDE_GANONS_CASTLE_COLLAPSE
};

void Sram_InitSram(void) {
    static const char* kMagic = "OoTMM-SAVE-START";

    SaveRaw_OptionsRead();
    if (memcmp(gSaveOptions.magic, kMagic, 16))
    {
        memcpy(gSaveOptions.magic, kMagic, 16);
        gSaveOptions.audioSetting = 0;
        gSaveOptions.zTargetSetting = 0;
        SaveRaw_OptionsWrite();
    }

    func_800F6700(gSaveOptions.audioSetting);
}

void Sram_OnLoad(void)
{
    u16 i;
    u16 j;
    u8* ptr;

    switch (gOotSave.info.playerData.savedSceneId) {
        case SCENE_DEKU_TREE:
        case SCENE_DODONGOS_CAVERN:
        case SCENE_JABU_JABU:
        case SCENE_FOREST_TEMPLE:
        case SCENE_FIRE_TEMPLE:
        case SCENE_WATER_TEMPLE:
        case SCENE_SPIRIT_TEMPLE:
        case SCENE_SHADOW_TEMPLE:
        case SCENE_BOTTOM_OF_THE_WELL:
        case SCENE_ICE_CAVERN:
        case SCENE_GANONS_TOWER:
        case SCENE_GERUDO_TRAINING_GROUND:
        case SCENE_THIEVES_HIDEOUT:
        case SCENE_INSIDE_GANONS_CASTLE:
            gOotSave.entranceIndex = sDungeonEntrances[gOotSave.info.playerData.savedSceneId];
            break;

        case SCENE_DEKU_TREE_BOSS:
            gOotSave.entranceIndex = ENTR_DEKU_TREE_0;
            break;

        case SCENE_DODONGOS_CAVERN_BOSS:
            gOotSave.entranceIndex = ENTR_DODONGOS_CAVERN_0;
            break;

        case SCENE_JABU_JABU_BOSS:
            gOotSave.entranceIndex = ENTR_JABU_JABU_0;
            break;

        case SCENE_FOREST_TEMPLE_BOSS:
            gOotSave.entranceIndex = ENTR_FOREST_TEMPLE_0;
            break;

        case SCENE_FIRE_TEMPLE_BOSS:
            gOotSave.entranceIndex = ENTR_FIRE_TEMPLE_0;
            break;

        case SCENE_WATER_TEMPLE_BOSS:
            gOotSave.entranceIndex = ENTR_WATER_TEMPLE_0;
            break;

        case SCENE_SPIRIT_TEMPLE_BOSS:
            gOotSave.entranceIndex = ENTR_SPIRIT_TEMPLE_0;
            break;

        case SCENE_SHADOW_TEMPLE_BOSS:
            gOotSave.entranceIndex = ENTR_SHADOW_TEMPLE_0;
            break;

        case SCENE_GANONS_TOWER_COLLAPSE_INTERIOR:
        case SCENE_INSIDE_GANONS_CASTLE_COLLAPSE:
        case SCENE_GANONDORF_BOSS:
        case SCENE_GANONS_TOWER_COLLAPSE_EXTERIOR:
        case SCENE_GANON_BOSS:
            gOotSave.entranceIndex = ENTR_GANONS_TOWER_0;
            break;

        default:
            if (gOotSave.info.playerData.savedSceneId != SCENE_LINKS_HOUSE) {
                if (LINK_AGE_IN_YEARS == YEARS_CHILD) {
                    gOotSave.entranceIndex = ENTR_LINKS_HOUSE_0;
                } else {
                    gOotSave.entranceIndex = ENTR_TEMPLE_OF_TIME_7;
                }
            } else {
                gOotSave.entranceIndex = ENTR_LINKS_HOUSE_0;
            }
            break;
    }

    if (gOotSave.info.playerData.health < 0x30) {
        gOotSave.info.playerData.health = 0x30;
    }

    if (gOotSave.info.scarecrowLongSongSet) {
        MemCpy(gScarecrowLongSongPtr, gOotSave.info.scarecrowLongSong, sizeof(gOotSave.info.scarecrowLongSong));
    }

    if (gOotSave.info.scarecrowSpawnSongSet) {
        MemCpy(gScarecrowSpawnSongPtr, gOotSave.info.scarecrowSpawnSong, sizeof(gOotSave.info.scarecrowSpawnSong));
    }

    if (LINK_AGE_IN_YEARS == YEARS_ADULT && !CHECK_OWNED_EQUIP(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER)) {
        gOotSave.info.inventory.equipment |= OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER);
#if OOT_VERSION >= NTSC_1_1
        gOotSave.info.equips.buttonItems[0] = ITEM_SWORD_MASTER;
        gOotSave.info.equips.equipment &= ~(0xF << (EQUIP_TYPE_SWORD * 4));
        gOotSave.info.equips.equipment |= EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4);
#endif
    }

    gOotSave.info.playerData.magicLevel = 0;

    gSaveContext.gameMode = GAMEMODE_NORMAL;
    gSaveContext.respawn[RESPAWN_MODE_DOWN].entranceIndex = ENTR_LOAD_OPENING;
    gSaveContext.respawnFlag = 0;
    gSaveContext.seqId = (u8)NA_BGM_DISABLED;
    gSaveContext.natureAmbienceId = 0xFF;
    gSaveContext.showTitleCard = true;
    gSaveContext.dogParams = 0;
    gSaveContext.timerState = TIMER_STATE_OFF;
    gSaveContext.subTimerState = SUBTIMER_STATE_OFF;
    gSaveContext.eventInf[0] = 0;
    gSaveContext.eventInf[1] = 0;
    gSaveContext.eventInf[2] = 0;
    gSaveContext.eventInf[3] = 0;
    gSaveContext.prevHudVisibilityMode = HUD_VISIBILITY_ALL;
    gSaveContext.nayrusLoveTimer = 0;
    gSaveContext.healthAccumulator = 0;
    gSaveContext.magicState = MAGIC_STATE_IDLE;
    gSaveContext.prevMagicState = MAGIC_STATE_IDLE;
    gSaveContext.forcedSeqId = NA_BGM_GENERAL_SFX;
    gSaveContext.skyboxTime = CLOCK_TIME(0, 0);
    gSaveContext.nextTransitionType = TRANS_NEXT_TYPE_DEFAULT;
    gSaveContext.nextCutsceneIndex = 0xFFEF;
    gSaveContext.cutsceneTrigger = 0;
    gSaveContext.chamberCutsceneNum = CHAMBER_CS_FOREST;
    gSaveContext.nextDayTime = NEXT_TIME_NONE;
    gSaveContext.retainWeatherMode = false;

    gSaveContext.buttonStatus[0] = gSaveContext.buttonStatus[1] = gSaveContext.buttonStatus[2] =
        gSaveContext.buttonStatus[3] = gSaveContext.buttonStatus[4] = BTN_ENABLED;

    gSaveContext.forceRisingButtonAlphas = gSaveContext.nextHudVisibilityMode = gSaveContext.hudVisibilityMode =
        gSaveContext.hudVisibilityModeTimer = gSaveContext.magicCapacity = 0; // false, HUD_VISIBILITY_NO_CHANGE

    // Set the fill target to be the saved magic amount
    gSaveContext.magicFillTarget = gOotSave.info.playerData.magic;
    // Set `magicLevel` and `magic` to 0 so `magicCapacity` then `magic` grows from nothing to respectively the full
    // capacity and `magicFillTarget`
    gOotSave.info.playerData.magicLevel = gOotSave.info.playerData.magic = 0;
    gOotSave.info.playerData.naviTimer = 0;
}
