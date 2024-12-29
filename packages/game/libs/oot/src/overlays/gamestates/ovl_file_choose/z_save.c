#include <string.h>
#include "file_select.h"
#include <combo.h>

static OotSavePlayerData sNewSavePlayerDataOot = {
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

static OotItemEquips sNewSaveEquipsOot = {
    { ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE }, // buttonItems
    { SLOT_NONE, SLOT_NONE, SLOT_NONE },            // cButtonSlots
    0x1100,                                         // equipment
};

static OotInventory sNewSaveInventoryOot = {
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

/**
 *  Initialize new save.
 *  This save has an empty inventory with 3 hearts and single magic.
 */
static void CreateSaveOot(void) {
    gOotSave.totalDays = 0;
    gOotSave.bgsDayCount = 0;

    gOotSave.info.playerData = sNewSavePlayerDataOot;
    gOotSave.info.equips = sNewSaveEquipsOot;
    gOotSave.info.inventory = sNewSaveInventoryOot;

    gOotSave.info.horseData.sceneId = SCENE_HYRULE_FIELD;
    gOotSave.info.horseData.pos.x = -1840;
    gOotSave.info.horseData.pos.y = 72;
    gOotSave.info.horseData.pos.z = 5497;
    gOotSave.info.horseData.angle = -0x6AD9;
    gOotSave.info.playerData.magicLevel = 0;
    gOotSave.info.infTable[INFTABLE_1DX_INDEX] = 1;
    gOotSave.info.sceneFlags[SCENE_WATER_TEMPLE].swch = 0x40000000;

    /* Skip Saria and Mido */
    gOotSave.info.eventChkInf[0] |= 0x102c;
    gOotSave.info.infTable[0] |= 0x000b;

    gOotSave.entranceIndex = ENTR_LINKS_HOUSE_0;
    gOotSave.linkAge = LINK_AGE_CHILD;
    gOotSave.dayTime = CLOCK_TIME(10, 0);
    gOotSave.cutsceneIndex = 0;
}


static const MmItemEquips kDefaultItemEquipsMm =
{
    {
        { 0xff, 0xff, 0xff, 0xff },
        { 0x4d, 0xff, 0xff, 0xff },
        { 0x4d, 0xff, 0xff, 0xff },
        { 0xfd, 0xff, 0xff, 0xff },
    },
    {
        { 0x00, 0xff, 0xff, 0xff },
        { 0xff, 0xff, 0xff, 0xff },
        { 0xff, 0xff, 0xff, 0xff },
        { 0xff, 0xff, 0xff, 0xff },
    },
    0,
};

static const MmInventory kDefaultInventoryMm =
{
    {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    },
    (1 << 17) | (1 << 20), // Deku nuts and sticks
    0,
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
    0,
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {
        { 0x15, 0x12, 0x17, 0x14, 0x3e, 0x3e, 0x3e, 0x3e },
        { 0x15, 0x12, 0x17, 0x14, 0x3e, 0x3e, 0x3e, 0x3e },
        { 0x15, 0x12, 0x17, 0x14, 0x3e, 0x3e, 0x3e, 0x3e },
    },
};

/* TODO: Move this */
#define MM_SET_EVENT_WEEK(x)        (gMmSave.saveInfo.weekEventReg[(x) / 8] |= (1 << ((x) % 8)))
#define EV_MM_WEEK_FIRST_CYCLE      MM_EV(59, 2)
#define MM_EV(a, b)                 (((a) << 3) | (b))

static void CreateSaveMm(void)
{
    int i;
    int j;
    u8 tmp;

    /* Create some save data */
    gMmSave.entrance = 0xd800; //ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER;
    gMmSave.cutsceneIndex = 0;
    gMmSave.isFirstCycle = 1;
    gMmSave.hasTatl = 1;
    gMmSave.playerForm = 4;
    gMmSave.time = 0x3fff;
    gMmSave.saveInfo.playerData.health = 0x30;
    gMmSave.saveInfo.playerData.healthCapacity = 0x30;
    gMmSave.saveInfo.playerData.savedSceneId = 8;
    gMmSave.saveInfo.horseData.sceneId = 53;
    gMmSave.saveInfo.horseData.pos.x = -1420;
    gMmSave.saveInfo.horseData.pos.y = 257;
    gMmSave.saveInfo.horseData.pos.z = -1285;
    gMmSave.saveInfo.horseData.yaw = -0x7554;

    /* Set the defaults */
    memcpy(&gMmSave.saveInfo.equips, &kDefaultItemEquipsMm, sizeof(gMmSave.saveInfo.equips));
    memcpy(&gMmSave.saveInfo.inventory, &kDefaultInventoryMm, sizeof(gMmSave.saveInfo.inventory));

    /* Set some events */
    MM_SET_EVENT_WEEK(EV_MM_WEEK_FIRST_CYCLE);

    /* Set the high scores */
    gMmSave.saveInfo.dekuPlaygroundHighScores[0] = 7500;
    gMmSave.saveInfo.dekuPlaygroundHighScores[1] = 7500;
    gMmSave.saveInfo.dekuPlaygroundHighScores[2] = 7600;
    gMmSave.saveInfo.highScores[4] = 6000;
    gMmSave.saveInfo.highScores[6] = (0x000a << 16) | 0x0027;

    /* Set the lottery codes */
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 3; ++j)
            gMmSave.saveInfo.lotteryCodes[i][j] = Rand_S16Offset(0, 10);
    }

    /* Set the bomber code */
    gMmSave.saveInfo.bomberCode[0] = 1;
    gMmSave.saveInfo.bomberCode[1] = 2;
    gMmSave.saveInfo.bomberCode[2] = 3;
    gMmSave.saveInfo.bomberCode[3] = 4;
    gMmSave.saveInfo.bomberCode[4] = 5;

    for (i = 0; i < 5; ++i)
    {
        j = Rand_S16Offset(0, 5);
        tmp = gMmSave.saveInfo.bomberCode[i];
        gMmSave.saveInfo.bomberCode[i] = gMmSave.saveInfo.bomberCode[j];
        gMmSave.saveInfo.bomberCode[j] = tmp;
    }

    /* Set the spider house masks order */
    tmp = 0xff;
    for (i = 0; i < 6; ++i)
    {
        do
        {
            gMmSave.saveInfo.spiderHouseMaskOrder[i] = Rand_S16Offset(0, 0x10) & 3;
        }
        while (gMmSave.saveInfo.spiderHouseMaskOrder[i] == tmp);
        tmp = gMmSave.saveInfo.spiderHouseMaskOrder[i];
    }
}

void FileSelect_CreateSave(FileSelectState* this, int index)
{
    /* Zero the save */
    memset(&gSave, 0, sizeof(gSave));

    /* Create the save */
    CreateSaveOot();
    CreateSaveMm();

    /* Copy the filename */
    memcpy(&gSave.oot.info.playerData.playerName[0], &this->fileNames[index][0], 8);

    /* Finish the save */
    memcpy(gSave.magic, SAVE_MAGIC, 16);

    /* Write the save */
    SaveRaw_WriteTo(index);

    /* Save is now valid */
    this->valid[this->buttonIndex] = 1;
}
