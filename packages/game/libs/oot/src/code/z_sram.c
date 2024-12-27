#include "global.h"
#include "terminal.h"
#include "versions.h"

#define SLOT_SIZE (sizeof(gOotSave) + 0x28)
#define CHECKSUM_SIZE (sizeof(gOotSave) / 2)

#define DEATHS offsetof(OotSave, info.playerData.deaths)
#define NAME offsetof(OotSave, info.playerData.playerName)
#define N64DD offsetof(OotSave, info.playerData.n64ddFlag)
#define HEALTH_CAP offsetof(OotSave, info.playerData.healthCapacity)
#define QUEST offsetof(OotSave, info.inventory.questItems)
#define DEFENSE offsetof(OotSave, info.inventory.defenseHearts)

#define SLOT_OFFSET(index) (SRAM_HEADER_SIZE + 0x10 + (index * SLOT_SIZE))

u16 gSramSlotOffsets[] = {
    SLOT_OFFSET(0),
    SLOT_OFFSET(1),
    SLOT_OFFSET(2),
    // the latter three saves are backup saves for the former saves
    SLOT_OFFSET(3),
    SLOT_OFFSET(4),
    SLOT_OFFSET(5),
};

static char sZeldaMagic[] = { '\0', '\0', '\0', '\x98', '\x09', '\x10', '\x21', 'Z', 'E', 'L', 'D', 'A' };

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
        if (gSaveContext.fileNum == 0xFF) {
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
void Sram_OpenSave(SramContext* sramCtx) {
    u16 i;
    u16 j;
    u8* ptr;

    PRINTF(T("個人Ｆｉｌｅ作成\n", "Create personal file\n"));
    i = gSramSlotOffsets[gSaveContext.fileNum];
    PRINTF(T("ぽいんと＝%x(%d)\n", "Point=%x(%d)\n"), i, gSaveContext.fileNum);

    MemCpy(&gOotSave, sramCtx->readBuff + i, sizeof(gOotSave));

    PRINTF(VT_FGCOL(YELLOW));
    PRINTF("SCENE_DATA_ID = %d   SceneNo = %d\n", gOotSave.info.playerData.savedSceneId,
           ((void)0, gOotSave.entranceIndex));

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

    PRINTF("scene_no = %d\n", gOotSave.entranceIndex);
    PRINTF(VT_RST);

    if (gOotSave.info.playerData.health < 0x30) {
        gOotSave.info.playerData.health = 0x30;
    }

    if (gOotSave.info.scarecrowLongSongSet) {
        PRINTF(VT_FGCOL(BLUE));
        PRINTF("\n====================================================================\n");

        MemCpy(gScarecrowLongSongPtr, gOotSave.info.scarecrowLongSong,
               sizeof(gOotSave.info.scarecrowLongSong));

        ptr = (u8*)gScarecrowLongSongPtr;
        for (i = 0; i < ARRAY_COUNT(gOotSave.info.scarecrowLongSong); i++, ptr++) {
            PRINTF("%d, ", *ptr);
        }

        PRINTF("\n====================================================================\n");
        PRINTF(VT_RST);
    }

    if (gOotSave.info.scarecrowSpawnSongSet) {
        PRINTF(VT_FGCOL(GREEN));
        PRINTF("\n====================================================================\n");

        MemCpy(gScarecrowSpawnSongPtr, gOotSave.info.scarecrowSpawnSong,
               sizeof(gOotSave.info.scarecrowSpawnSong));

        ptr = gScarecrowSpawnSongPtr;
        for (i = 0; i < ARRAY_COUNT(gOotSave.info.scarecrowSpawnSong); i++, ptr++) {
            PRINTF("%d, ", *ptr);
        }

        PRINTF("\n====================================================================\n");
        PRINTF(VT_RST);
    }

    // if zelda cutscene has been watched but lullaby was not obtained, restore cutscene and take away letter
    if (GET_EVENTCHKINF(EVENTCHKINF_40) && !CHECK_QUEST_ITEM(QUEST_SONG_LULLABY)) {
        i = gOotSave.info.eventChkInf[EVENTCHKINF_40_INDEX] & ~EVENTCHKINF_40_MASK;
        gOotSave.info.eventChkInf[EVENTCHKINF_40_INDEX] = i;

        INV_CONTENT(ITEM_ZELDAS_LETTER) = ITEM_CHICKEN;

        for (j = 1; j < 4; j++) {
            if (gOotSave.info.equips.buttonItems[j] == ITEM_ZELDAS_LETTER) {
                gOotSave.info.equips.buttonItems[j] = ITEM_CHICKEN;
            }
        }
    }

    if (LINK_AGE_IN_YEARS == YEARS_ADULT && !CHECK_OWNED_EQUIP(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER)) {
        gOotSave.info.inventory.equipment |= OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER);
#if OOT_VERSION >= NTSC_1_1
        gOotSave.info.equips.buttonItems[0] = ITEM_SWORD_MASTER;
        gOotSave.info.equips.equipment &= ~(0xF << (EQUIP_TYPE_SWORD * 4));
        gOotSave.info.equips.equipment |= EQUIP_VALUE_SWORD_MASTER << (EQUIP_TYPE_SWORD * 4);
#endif
    }

    for (i = 0; i < ARRAY_COUNT(gSpoilingItems); i++) {
        if (INV_CONTENT(ITEM_TRADE_ADULT) == gSpoilingItems[i]) {
            INV_CONTENT(gSpoilingItemReverts[i]) = gSpoilingItemReverts[i];

            for (j = 1; j < 4; j++) {
                if (gOotSave.info.equips.buttonItems[j] == gSpoilingItems[i]) {
                    gOotSave.info.equips.buttonItems[j] = gSpoilingItemReverts[i];
                }
            }
        }
    }

    gOotSave.info.playerData.magicLevel = 0;
}

/**
 *  Write the contents of the Save Context to a main and backup slot in SRAM.
 *  Note: The whole Save Context is written even though only the `save` substruct is read back later
 */
void Sram_WriteSave(SramContext* sramCtx) {
    u16 offset;
    u16 checksum;
    u16 j;
    u16* ptr;

    gOotSave.info.checksum = 0;

    ptr = (u16*)&gOotSave;
    checksum = j = 0;

    for (offset = 0; offset < CHECKSUM_SIZE; offset++) {
        if (++j == 0x20) {
            j = 0;
        }
        checksum += *ptr++;
    }

    gOotSave.info.checksum = checksum;

    ptr = (u16*)&gOotSave;
    checksum = 0;

    for (offset = 0; offset < CHECKSUM_SIZE; offset++) {
        if (++j == 0x20) {
            j = 0;
        }
        checksum += *ptr++;
    }

    offset = gSramSlotOffsets[gSaveContext.fileNum];
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + offset, &gOotSave, SLOT_SIZE, OS_WRITE);

    ptr = (u16*)&gOotSave;
    checksum = 0;

    for (offset = 0; offset < CHECKSUM_SIZE; offset++) {
        if (++j == 0x20) {
            j = 0;
        }
        checksum += *ptr++;
    }

    offset = gSramSlotOffsets[gSaveContext.fileNum + 3];
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + offset, &gOotSave, SLOT_SIZE, OS_WRITE);
}

/**
 *  For all 3 slots, verify that the checksum is correct. If corrupted, attempt to load a backup save.
 *  If backup is also corrupted, default to a new save (or debug save for slot 0 on debug rom).
 *
 *  After verifying all 3 saves, pass relevant data to File Select to be displayed.
 */
void Sram_VerifyAndLoadAllSaves(FileSelectState* fileSelect, SramContext* sramCtx) {
    u16 i;
    u16 newChecksum;
    u16 slotNum;
    u16 offset;
    u16 j;
    u16 oldChecksum;
    u16* ptr;
    u16 dayTime;

    PRINTF("ＳＲＡＭ ＳＴＡＲＴ─ＬＯＡＤ\n");
    bzero(sramCtx->readBuff, SRAM_SIZE);
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_READ);

    dayTime = ((void)0, gOotSave.dayTime);

    for (slotNum = 0; slotNum < 3; slotNum++) {
        offset = gSramSlotOffsets[slotNum];
        MemCpy(&gOotSave, sramCtx->readBuff + offset, sizeof(gOotSave));

        oldChecksum = gOotSave.info.checksum;
        gOotSave.info.checksum = 0;
        ptr = (u16*)&gOotSave;
        PRINTF("\n＝＝＝＝＝＝＝＝＝＝＝＝＝  Ｓ（%d） ＝＝＝＝＝＝＝＝＝＝＝＝＝\n", slotNum);

        for (i = newChecksum = j = 0; i < CHECKSUM_SIZE; i++, offset += 2) {
#if OOT_VERSION < PAL_1_0
            if (j) {}
            j += 2;
            if (j == 0x20) {
                j = 0;
            }
#endif
            newChecksum += *ptr++;
        }

        PRINTF(T("\nＳＡＶＥチェックサム計算  j=%x  mmm=%x  ", "\nSAVE checksum calculation  j=%x  mmm=%x  "),
               newChecksum, oldChecksum);

        if (newChecksum != oldChecksum) {
            // checksum didnt match, try backup save
            PRINTF("ＥＲＲＯＲ！！！ ＝ %x(%d)\n", gSramSlotOffsets[slotNum], slotNum);
            offset = gSramSlotOffsets[slotNum + 3];
            MemCpy(&gOotSave, sramCtx->readBuff + offset, sizeof(gOotSave));

            oldChecksum = gOotSave.info.checksum;
            gOotSave.info.checksum = 0;
            ptr = (u16*)&gOotSave;
            PRINTF("================= ＢＡＣＫ─ＵＰ ========================\n");

            for (i = newChecksum = j = 0; i < CHECKSUM_SIZE; i++, offset += 2) {
#if OOT_VERSION < PAL_1_0
                if (j) {}
                j += 2;
                if (j == 0x20) {
                    j = 0;
                }
#endif
                newChecksum += *ptr++;
            }
            PRINTF(T("\n（Ｂ）ＳＡＶＥチェックサム計算  j=%x  mmm=%x  ",
                     "\n(B) SAVE checksum calculation  j=%x  mmm=%x  "),
                   newChecksum, oldChecksum);

            if (newChecksum != oldChecksum) {
                // backup save didnt work, make new save
                PRINTF("ＥＲＲＯＲ！！！ ＝ %x(%d+3)\n", gSramSlotOffsets[slotNum + 3], slotNum);
                bzero(&gOotSave.entranceIndex, sizeof(s32));
                bzero(&gOotSave.linkAge, sizeof(s32));
                bzero(&gOotSave.cutsceneIndex, sizeof(s32));
                //! @bug gOotSave.dayTime is a u16 but is cleared as a 32-bit value. This is harmless as-is
                //! since it is followed by nightFlag which is also reset here, but can become an issue if the save
                //! layout is changed.
                bzero(&gOotSave.dayTime, sizeof(s32));
                bzero(&gOotSave.nightFlag, sizeof(s32));
                bzero(&gOotSave.totalDays, sizeof(s32));
                bzero(&gOotSave.bgsDayCount, sizeof(s32));

#if DEBUG_FEATURES
                if (!slotNum) {
                    Sram_InitDebugSave();
                    gOotSave.info.playerData.newf[0] = 'Z';
                    gOotSave.info.playerData.newf[1] = 'E';
                    gOotSave.info.playerData.newf[2] = 'L';
                    gOotSave.info.playerData.newf[3] = 'D';
                    gOotSave.info.playerData.newf[4] = 'A';
                    gOotSave.info.playerData.newf[5] = 'Z';
                    PRINTF("newf=%x,%x,%x,%x,%x,%x\n", gOotSave.info.playerData.newf[0],
                           gOotSave.info.playerData.newf[1], gOotSave.info.playerData.newf[2],
                           gOotSave.info.playerData.newf[3], gOotSave.info.playerData.newf[4],
                           gOotSave.info.playerData.newf[5]);
                } else {
                    Sram_InitNewSave();
                }
#else
                Sram_InitNewSave();
#endif

                ptr = (u16*)&gOotSave;
                PRINTF("\n--------------------------------------------------------------\n");

                for (i = newChecksum = j = 0; i < CHECKSUM_SIZE; i++) {
                    PRINTF("%x ", *ptr);
                    if (++j == 0x20) {
                        PRINTF("\n");
                        j = 0;
                    }
                    newChecksum += *ptr++;
                }

                gOotSave.info.checksum = newChecksum;
                PRINTF("\nCheck_Sum=%x(%x)\n", gOotSave.info.checksum, newChecksum);

                i = gSramSlotOffsets[slotNum + 3];
                SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + i, &gOotSave, SLOT_SIZE, OS_WRITE);

                PRINTF("????#%x,%x,%x,%x,%x,%x\n", gOotSave.info.playerData.newf[0],
                       gOotSave.info.playerData.newf[1], gOotSave.info.playerData.newf[2],
                       gOotSave.info.playerData.newf[3], gOotSave.info.playerData.newf[4],
                       gOotSave.info.playerData.newf[5]);
                PRINTF(T("\nぽいんと＝%x(%d+3)  check_sum=%x(%x)\n", "\npoints=%x(%d+3) check_sum=%x(%x)\n"), i,
                       slotNum, gOotSave.info.checksum, newChecksum);
            }

            i = gSramSlotOffsets[slotNum];
            SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + i, &gOotSave, SLOT_SIZE, OS_WRITE);

            PRINTF(T("ぽいんと＝%x(%d)  check_sum=%x(%x)\n", "point=%x(%d) check_sum=%x(%x)\n"), i, slotNum,
                   gOotSave.info.checksum, newChecksum);
        } else {
            PRINTF(T("\nＳＡＶＥデータ ＯＫ！！！！\n", "\nSAVE data OK!!!!\n"));
        }
    }

    bzero(sramCtx->readBuff, SRAM_SIZE);
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_READ);
    gOotSave.dayTime = dayTime;

    PRINTF("SAVECT=%x, NAME=%x, LIFE=%x, ITEM=%x,  64DD=%x,  HEART=%x\n", DEATHS, NAME, HEALTH_CAP, QUEST, N64DD,
           DEFENSE);

    MemCpy(&fileSelect->deaths[0], sramCtx->readBuff + SLOT_OFFSET(0) + DEATHS, sizeof(fileSelect->deaths[0]));
    MemCpy(&fileSelect->deaths[1], sramCtx->readBuff + SLOT_OFFSET(1) + DEATHS, sizeof(fileSelect->deaths[0]));
    MemCpy(&fileSelect->deaths[2], sramCtx->readBuff + SLOT_OFFSET(2) + DEATHS, sizeof(fileSelect->deaths[0]));

    MemCpy(&fileSelect->fileNames[0], sramCtx->readBuff + SLOT_OFFSET(0) + NAME, sizeof(fileSelect->fileNames[0]));
    MemCpy(&fileSelect->fileNames[1], sramCtx->readBuff + SLOT_OFFSET(1) + NAME, sizeof(fileSelect->fileNames[0]));
    MemCpy(&fileSelect->fileNames[2], sramCtx->readBuff + SLOT_OFFSET(2) + NAME, sizeof(fileSelect->fileNames[0]));

    MemCpy(&fileSelect->healthCapacities[0], sramCtx->readBuff + SLOT_OFFSET(0) + HEALTH_CAP,
           sizeof(fileSelect->healthCapacities[0]));
    MemCpy(&fileSelect->healthCapacities[1], sramCtx->readBuff + SLOT_OFFSET(1) + HEALTH_CAP,
           sizeof(fileSelect->healthCapacities[0]));
    MemCpy(&fileSelect->healthCapacities[2], sramCtx->readBuff + SLOT_OFFSET(2) + HEALTH_CAP,
           sizeof(fileSelect->healthCapacities[0]));

    MemCpy(&fileSelect->questItems[0], sramCtx->readBuff + SLOT_OFFSET(0) + QUEST, sizeof(fileSelect->questItems[0]));
    MemCpy(&fileSelect->questItems[1], sramCtx->readBuff + SLOT_OFFSET(1) + QUEST, sizeof(fileSelect->questItems[0]));
    MemCpy(&fileSelect->questItems[2], sramCtx->readBuff + SLOT_OFFSET(2) + QUEST, sizeof(fileSelect->questItems[0]));

    MemCpy(&fileSelect->n64ddFlags[0], sramCtx->readBuff + SLOT_OFFSET(0) + N64DD, sizeof(fileSelect->n64ddFlags[0]));
    MemCpy(&fileSelect->n64ddFlags[1], sramCtx->readBuff + SLOT_OFFSET(1) + N64DD, sizeof(fileSelect->n64ddFlags[0]));
    MemCpy(&fileSelect->n64ddFlags[2], sramCtx->readBuff + SLOT_OFFSET(2) + N64DD, sizeof(fileSelect->n64ddFlags[0]));

    MemCpy(&fileSelect->defense[0], sramCtx->readBuff + SLOT_OFFSET(0) + DEFENSE, sizeof(fileSelect->defense[0]));
    MemCpy(&fileSelect->defense[1], sramCtx->readBuff + SLOT_OFFSET(1) + DEFENSE, sizeof(fileSelect->defense[0]));
    MemCpy(&fileSelect->defense[2], sramCtx->readBuff + SLOT_OFFSET(2) + DEFENSE, sizeof(fileSelect->defense[0]));

#if OOT_PAL
    MemCpy(&fileSelect->health[0], sramCtx->readBuff + SLOT_OFFSET(0) + HEALTH, sizeof(fileSelect->health[0]));
    MemCpy(&fileSelect->health[1], sramCtx->readBuff + SLOT_OFFSET(1) + HEALTH, sizeof(fileSelect->health[0]));
    MemCpy(&fileSelect->health[2], sramCtx->readBuff + SLOT_OFFSET(2) + HEALTH, sizeof(fileSelect->health[0]));
#endif

    PRINTF("f_64dd=%d, %d, %d\n", fileSelect->n64ddFlags[0], fileSelect->n64ddFlags[1], fileSelect->n64ddFlags[2]);
    PRINTF("heart_status=%d, %d, %d\n", fileSelect->defense[0], fileSelect->defense[1], fileSelect->defense[2]);
#if OOT_PAL
    PRINTF("now_life=%d, %d, %d\n", fileSelect->health[0], fileSelect->health[1], fileSelect->health[2]);
#endif
}

void Sram_InitSave(FileSelectState* fileSelect, SramContext* sramCtx) {
    u16 offset;
    u16 j;
    u16* ptr;
    u16 checksum;

    Sram_InitNewSave();

    gOotSave.entranceIndex = ENTR_LINKS_HOUSE_0;
    gOotSave.linkAge = LINK_AGE_CHILD;
    gOotSave.dayTime = CLOCK_TIME(10, 0);
    gOotSave.cutsceneIndex = 0;

    for (offset = 0; offset < 8; offset++) {
        gOotSave.info.playerData.playerName[offset] = fileSelect->fileNames[fileSelect->buttonIndex][offset];
    }

    gOotSave.info.playerData.newf[0] = 'Z';
    gOotSave.info.playerData.newf[1] = 'E';
    gOotSave.info.playerData.newf[2] = 'L';
    gOotSave.info.playerData.newf[3] = 'D';
    gOotSave.info.playerData.newf[4] = 'A';
    gOotSave.info.playerData.newf[5] = 'Z';

    gOotSave.info.playerData.n64ddFlag = fileSelect->n64ddFlag;

    ptr = (u16*)&gOotSave;

    for (j = 0, checksum = 0, offset = 0; offset < CHECKSUM_SIZE; offset++) {
        PRINTF("%x ", *ptr);
        checksum += *ptr++;
        if (++j == 0x20) {
            PRINTF("\n");
            j = 0;
        }
    }

    gOotSave.info.checksum = checksum;
    PRINTF(T("\nチェックサム＝%x\n", "\nChecksum = %x\n"), gOotSave.info.checksum);

    offset = gSramSlotOffsets[gSaveContext.fileNum];
    PRINTF("I=%x no=%d\n", offset, gSaveContext.fileNum);
    MemCpy(sramCtx->readBuff + offset, &gOotSave, sizeof(gOotSave));

    offset = gSramSlotOffsets[gSaveContext.fileNum + 3];
    PRINTF("I=%x no=%d\n", offset, gSaveContext.fileNum + 3);
    MemCpy(sramCtx->readBuff + offset, &gOotSave, sizeof(gOotSave));

    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_WRITE);

    PRINTF(T("ＳＡＶＥ終了\n", "SAVE end\n"));
    PRINTF("z_common_data.file_no = %d\n", gSaveContext.fileNum);
    PRINTF("SAVECT=%x, NAME=%x, LIFE=%x, ITEM=%x,  SAVE_64DD=%x\n", DEATHS, NAME, HEALTH_CAP, QUEST, N64DD);

    j = gSramSlotOffsets[gSaveContext.fileNum];

    MemCpy(&fileSelect->deaths[gSaveContext.fileNum], sramCtx->readBuff + j + DEATHS, sizeof(fileSelect->deaths[0]));
    MemCpy(&fileSelect->fileNames[gSaveContext.fileNum], sramCtx->readBuff + j + NAME,
           sizeof(fileSelect->fileNames[0]));
    MemCpy(&fileSelect->healthCapacities[gSaveContext.fileNum], sramCtx->readBuff + j + HEALTH_CAP,
           sizeof(fileSelect->healthCapacities[0]));
    MemCpy(&fileSelect->questItems[gSaveContext.fileNum], sramCtx->readBuff + j + QUEST,
           sizeof(fileSelect->questItems[0]));
    MemCpy(&fileSelect->n64ddFlags[gSaveContext.fileNum], sramCtx->readBuff + j + N64DD,
           sizeof(fileSelect->n64ddFlags[0]));
    MemCpy(&fileSelect->defense[gSaveContext.fileNum], sramCtx->readBuff + j + DEFENSE, sizeof(fileSelect->defense[0]));
#if OOT_PAL
    MemCpy(&fileSelect->health[gSaveContext.fileNum], sramCtx->readBuff + j + HEALTH, sizeof(fileSelect->health[0]));
#endif

    PRINTF("f_64dd[%d]=%d\n", gSaveContext.fileNum, fileSelect->n64ddFlags[gSaveContext.fileNum]);
    PRINTF("heart_status[%d]=%d\n", gSaveContext.fileNum, fileSelect->defense[gSaveContext.fileNum]);
#if OOT_PAL
    PRINTF("now_life[%d]=%d\n", gSaveContext.fileNum, fileSelect->health[gSaveContext.fileNum]);
#endif
}

void Sram_EraseSave(FileSelectState* fileSelect, SramContext* sramCtx) {
    s32 offset;

    Sram_InitNewSave();

    offset = gSramSlotOffsets[fileSelect->selectedFileIndex];
    MemCpy(sramCtx->readBuff + offset, &gOotSave, sizeof(gOotSave));
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + offset, &gOotSave, SLOT_SIZE, OS_WRITE);

    MemCpy(&fileSelect->n64ddFlags[fileSelect->selectedFileIndex], sramCtx->readBuff + offset + N64DD,
           sizeof(fileSelect->n64ddFlags[0]));

    offset = gSramSlotOffsets[fileSelect->selectedFileIndex + 3];
    MemCpy(sramCtx->readBuff + offset, &gOotSave, sizeof(gOotSave));
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000) + offset, &gOotSave, SLOT_SIZE, OS_WRITE);

    PRINTF(T("ＣＬＥＡＲ終了\n", "CLEAR END\n"));
}

void Sram_CopySave(FileSelectState* fileSelect, SramContext* sramCtx) {
    s32 offset;

    PRINTF("ＲＥＡＤ=%d(%x)  ＣＯＰＹ=%d(%x)\n", fileSelect->selectedFileIndex,
           gSramSlotOffsets[fileSelect->selectedFileIndex], fileSelect->copyDestFileIndex,
           gSramSlotOffsets[fileSelect->copyDestFileIndex]);

    offset = gSramSlotOffsets[fileSelect->selectedFileIndex];
    MemCpy(&gOotSave, sramCtx->readBuff + offset, sizeof(gOotSave));

    offset = gSramSlotOffsets[fileSelect->copyDestFileIndex];
    MemCpy(sramCtx->readBuff + offset, &gOotSave, sizeof(gOotSave));

    offset = gSramSlotOffsets[fileSelect->copyDestFileIndex + 3];
    MemCpy(sramCtx->readBuff + offset, &gOotSave, sizeof(gOotSave));

    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_WRITE);

    offset = gSramSlotOffsets[fileSelect->copyDestFileIndex];

    MemCpy(&fileSelect->deaths[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + DEATHS,
           sizeof(fileSelect->deaths[0]));
    MemCpy(&fileSelect->fileNames[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + NAME,
           sizeof(fileSelect->fileNames[0]));
    MemCpy(&fileSelect->healthCapacities[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + HEALTH_CAP,
           sizeof(fileSelect->healthCapacities[0]));
    MemCpy(&fileSelect->questItems[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + QUEST,
           sizeof(fileSelect->questItems[0]));
    MemCpy(&fileSelect->n64ddFlags[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + N64DD,
           sizeof(fileSelect->n64ddFlags[0]));
    MemCpy(&fileSelect->defense[fileSelect->copyDestFileIndex], sramCtx->readBuff + offset + DEFENSE,
           sizeof(fileSelect->defense[0]));
#if OOT_PAL
    MemCpy(&fileSelect->health[fileSelect->copyDestFileIndex], (sramCtx->readBuff + offset) + HEALTH,
           sizeof(fileSelect->health[0]));
#endif

    PRINTF("f_64dd[%d]=%d\n", gSaveContext.fileNum, fileSelect->n64ddFlags[gSaveContext.fileNum]);
    PRINTF("heart_status[%d]=%d\n", gSaveContext.fileNum, fileSelect->defense[gSaveContext.fileNum]);
    PRINTF(T("ＣＯＰＹ終了\n", "Copy end\n"));
}

/**
 *  Write the first 16 bytes of the read buffer to the SRAM header
 */
void Sram_WriteSramHeader(SramContext* sramCtx) {
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_HEADER_SIZE, OS_WRITE);
}

void Sram_InitSram(GameState* gameState, SramContext* sramCtx) {
    u16 i;

    PRINTF("sram_initialize( Game *game, Sram *sram )\n");
    SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_READ);

    for (i = 0; i < ARRAY_COUNTU(sZeldaMagic) - 3; i++) {
        if (sZeldaMagic[i + SRAM_HEADER_MAGIC] != sramCtx->readBuff[i + SRAM_HEADER_MAGIC]) {
            PRINTF(T("ＳＲＡＭ破壊！！！！！！\n", "SRAM destruction!!!!!!\n"));
#if PLATFORM_GC && OOT_PAL
            gSaveContext.language = sramCtx->readBuff[SRAM_HEADER_LANGUAGE];
#endif

            MemCpy(sramCtx->readBuff, sZeldaMagic, sizeof(sZeldaMagic));

#if PLATFORM_GC && OOT_PAL
            sramCtx->readBuff[SRAM_HEADER_LANGUAGE] = gSaveContext.language;
#endif
            Sram_WriteSramHeader(sramCtx);
        }
    }

    gSaveContext.audioSetting = sramCtx->readBuff[SRAM_HEADER_SOUND] & 3;
    gSaveContext.zTargetSetting = sramCtx->readBuff[SRAM_HEADER_ZTARGET] & 1;

#if OOT_PAL
    gSaveContext.language = sramCtx->readBuff[SRAM_HEADER_LANGUAGE];
    if (gSaveContext.language >= LANGUAGE_MAX) {
        gSaveContext.language = LANGUAGE_ENG;
        sramCtx->readBuff[SRAM_HEADER_LANGUAGE] = gSaveContext.language;
        Sram_WriteSramHeader(sramCtx);
    }
#endif

#if DEBUG_FEATURES
    if (CHECK_BTN_ANY(gameState->input[2].cur.button, BTN_DRIGHT)) {
        bzero(sramCtx->readBuff, SRAM_SIZE);
        for (i = 0; i < CHECKSUM_SIZE; i++) {
            sramCtx->readBuff[i] = i;
        }
        SsSram_ReadWrite(OS_K1_TO_PHYSICAL(0xA8000000), sramCtx->readBuff, SRAM_SIZE, OS_WRITE);
        PRINTF(T("ＳＲＡＭ破壊！！！！！！\n", "SRAM destruction!!!!!!\n"));
    }
#endif
    func_800F6700(gSaveContext.audioSetting);
}

void Sram_Alloc(GameState* gameState, SramContext* sramCtx) {
    sramCtx->readBuff = GAME_STATE_ALLOC(gameState, SRAM_SIZE, "../z_sram.c", 1294);
    ASSERT(sramCtx->readBuff != NULL, "sram->read_buff != NULL", "../z_sram.c", 1295);
}

void Sram_Init(PlayState* play, SramContext* sramCtx) {
}
