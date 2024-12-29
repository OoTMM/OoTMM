#include "global.h"
#include "terminal.h"
#include "versions.h"

static OotSavePlayerData sNewSavePlayerData = {
    { '\0', '\0', '\0', '\0', '\0', '\0' }, // newf
    0,                                      // deaths
    {
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
        FILENAME_SPACE,
    },                  // playerName
    0,                  // n64ddFlag
    0x30,               // healthCapacity
    0x30,               // defense
    0,                  // magicLevel
    MAGIC_NORMAL_METER, // magic
    0,                  // rupees
    0,                  // swordHealth
    0,                  // naviTimer
    false,              // isMagicAcquired
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
    SCENE_LINKS_HOUSE,                                  // savedSceneId
};

static OotItemEquips sNewSaveEquips = {
    { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
    { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
    0x1100,                                         // equipment
};

static OotInventory sNewSaveInventory = {
    // items
    {
        ITEM_NONE, // SLOT_DEKU_STICK
        ITEM_NONE, // SLOT_DEKU_NUT
        ITEM_NONE, // SLOT_BOMB
        ITEM_NONE, // SLOT_BOW
        ITEM_NONE, // SLOT_ARROW_FIRE
        ITEM_NONE, // SLOT_DINS_FIRE
        ITEM_NONE, // SLOT_SLINGSHOT
        ITEM_NONE, // SLOT_OCARINA
        ITEM_NONE, // SLOT_BOMBCHU
        ITEM_NONE, // SLOT_HOOKSHOT
        ITEM_NONE, // SLOT_ARROW_ICE
        ITEM_NONE, // SLOT_FARORES_WIND
        ITEM_NONE, // SLOT_BOOMERANG
        ITEM_NONE, // SLOT_LENS_OF_TRUTH
        ITEM_NONE, // SLOT_MAGIC_BEAN
        ITEM_NONE, // SLOT_HAMMER
        ITEM_NONE, // SLOT_ARROW_LIGHT
        ITEM_NONE, // SLOT_NAYRUS_LOVE
        ITEM_NONE, // SLOT_BOTTLE_1
        ITEM_NONE, // SLOT_BOTTLE_2
        ITEM_NONE, // SLOT_BOTTLE_3
        ITEM_NONE, // SLOT_BOTTLE_4
        ITEM_NONE, // SLOT_TRADE_ADULT
        ITEM_NONE, // SLOT_TRADE_CHILD
    },
    // ammo
    {
        0, // SLOT_DEKU_STICK
        0, // SLOT_DEKU_NUT
        0, // SLOT_BOMB
        0, // SLOT_BOW
        0, // SLOT_ARROW_FIRE
        0, // SLOT_DINS_FIRE
        0, // SLOT_SLINGSHOT
        0, // SLOT_OCARINA
        0, // SLOT_BOMBCHU
        0, // SLOT_HOOKSHOT
        0, // SLOT_ARROW_ICE
        0, // SLOT_FARORES_WIND
        0, // SLOT_BOOMERANG
        0, // SLOT_LENS_OF_TRUTH
        0, // SLOT_MAGIC_BEAN
        0, // SLOT_HAMMER
    },
    // equipment
    (((1 << EQUIP_INV_TUNIC_KOKIRI) << (EQUIP_TYPE_TUNIC * 4)) |
     ((1 << EQUIP_INV_BOOTS_KOKIRI) << (EQUIP_TYPE_BOOTS * 4))),
    0,                                                              // upgrades
    0,                                                              // questItems
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // dungeonItems
    {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    }, // dungeonKeys
    0, // defenseHearts
    0, // gsTokens
};

static u16 sNewSaveChecksum = 0;

/**
 *  Initialize new save.
 *  This save has an empty inventory with 3 hearts and single magic.
 */
void Sram_InitNewSave(void) {
    bzero(&gOotSave.info, sizeof(OotSaveInfo));
    gOotSave.totalDays = 0;
    gOotSave.bgsDayCount = 0;

    gOotSave.info.playerData = sNewSavePlayerData;
    gOotSave.info.equips = sNewSaveEquips;
    gOotSave.info.inventory = sNewSaveInventory;

    gOotSave.info.checksum = sNewSaveChecksum;
    gOotSave.info.horseData.sceneId = SCENE_HYRULE_FIELD;
    gOotSave.info.horseData.pos.x = -1840;
    gOotSave.info.horseData.pos.y = 72;
    gOotSave.info.horseData.pos.z = 5497;
    gOotSave.info.horseData.angle = -0x6AD9;
    gOotSave.info.playerData.magicLevel = 0;
    gOotSave.info.infTable[INFTABLE_1DX_INDEX] = 1;
    gOotSave.info.sceneFlags[SCENE_WATER_TEMPLE].swch = 0x40000000;

    gOotSave.info.eventChkInf[0] |= 0x102c;
    gOotSave.info.infTable[0] |= 0x000b;
}

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
    { SLOT_BOW, SLOT_BOMB, SLOT_OCARINA },                          // cButtonSlots
    // equipment
    (EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4)) | (EQUIP_VALUE_SHIELD_HYLIAN << (EQUIP_TYPE_SHIELD * 4)) |
        (EQUIP_VALUE_TUNIC_KOKIRI << (EQUIP_TYPE_TUNIC * 4)) | (EQUIP_VALUE_BOOTS_KOKIRI << (EQUIP_TYPE_BOOTS * 4)),
};

static OotInventory sDebugSaveInventory = {
    // items
    {
        ITEM_DEKU_STICK,          // SLOT_DEKU_STICK
        ITEM_DEKU_NUT,            // SLOT_DEKU_NUT
        ITEM_BOMB,                // SLOT_BOMB
        ITEM_BOW,                 // SLOT_BOW
        ITEM_ARROW_FIRE,          // SLOT_ARROW_FIRE
        ITEM_DINS_FIRE,           // SLOT_DINS_FIRE
        ITEM_SLINGSHOT,           // SLOT_SLINGSHOT
        ITEM_OCARINA_FAIRY,       // SLOT_OCARINA
        ITEM_BOMBCHU,             // SLOT_BOMBCHU
        ITEM_HOOKSHOT,            // SLOT_HOOKSHOT
        ITEM_ARROW_ICE,           // SLOT_ARROW_ICE
        ITEM_FARORES_WIND,        // SLOT_FARORES_WIND
        ITEM_BOOMERANG,           // SLOT_BOOMERANG
        ITEM_LENS_OF_TRUTH,       // SLOT_LENS_OF_TRUTH
        ITEM_MAGIC_BEAN,          // SLOT_MAGIC_BEAN
        ITEM_HAMMER,              // SLOT_HAMMER
        ITEM_ARROW_LIGHT,         // SLOT_ARROW_LIGHT
        ITEM_NAYRUS_LOVE,         // SLOT_NAYRUS_LOVE
        ITEM_BOTTLE_EMPTY,        // SLOT_BOTTLE_1
        ITEM_BOTTLE_POTION_RED,   // SLOT_BOTTLE_2
        ITEM_BOTTLE_POTION_GREEN, // SLOT_BOTTLE_3
        ITEM_BOTTLE_POTION_BLUE,  // SLOT_BOTTLE_4
        ITEM_POCKET_EGG,          // SLOT_TRADE_ADULT
        ITEM_WEIRD_EGG,           // SLOT_TRADE_CHILD
    },
    // ammo
    {
        50, // SLOT_DEKU_STICK
        50, // SLOT_DEKU_NUT
        10, // SLOT_BOMB
        30, // SLOT_BOW
        1,  // SLOT_ARROW_FIRE
        1,  // SLOT_DINS_FIRE
        30, // SLOT_SLINGSHOT
        1,  // SLOT_OCARINA
        50, // SLOT_BOMBCHU
        1,  // SLOT_HOOKSHOT
        1,  // SLOT_ARROW_ICE
        1,  // SLOT_FARORES_WIND
        1,  // SLOT_BOOMERANG
        1,  // SLOT_LENS_OF_TRUTH
        1,  // SLOT_MAGIC_BEAN
        1   // SLOT_HAMMER
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

static u16 sDebugSaveChecksum = 0;

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
            gOotSave.info.equips.cButtonSlots[0] = SLOT_SLINGSHOT;
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

/**
 *  Copy save currently on the buffer to Save Context and complete various tasks to open the save.
 *  This includes:
 *  - Set proper entrance depending on where the game was saved
 *  - If health is less than 3 hearts, give 3 hearts
 *  - If either scarecrow song is set, copy them from save context to the proper location
 *  - Handle a case where the player saved and quit after zelda cutscene but didnt get the song
 *  - Give and equip master sword if player is adult and doesn't have master sword
 *  - Revert any trade items that spoil
 */
void Sram_OpenSave(void) {
    u16 i;
    u16 j;
    u8* ptr;

    SaveRaw_Read();

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
                gOotSave.entranceIndex =
                    (LINK_AGE_IN_YEARS == YEARS_CHILD) ? ENTR_LINKS_HOUSE_0 : ENTR_TEMPLE_OF_TIME_7;
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
}

/**
 *  Write the contents of the Save Context to a main and backup slot in SRAM.
 *  Note: The whole Save Context is written even though only the `save` substruct is read back later
 */
void Sram_WriteSave(void) {
    SaveRaw_Write();
}

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
