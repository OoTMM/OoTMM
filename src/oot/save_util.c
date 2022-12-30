#include <combo.h>
#include <string.h>

static void copyName(u8* dst, const u8* src)
{
    u8 c0;
    u8 c1;

    for (int i = 0; i < 8; ++i)
    {
        c0 = src[i];
        if (c0 <= 0x09)
            c1 = c0;
        else if (c0 >= 0xab && c0 < 0xab + 26)
            c1 = c0 - 0xab + 0x0a;
        else if (c0 >= 0xc5 && c0 < 0xc5 + 26)
            c1 = c0 - 0xc5 + 0x24;
        else if (c0 == 0xe4)
            c1 = 0x3f;
        else if (c0 == 0xea)
            c1 = 0x40;
        else
            c1 = 0x3e;
        dst[i] = c1;
    }
}

static const MmItemEquips kDefaultItemEquips =
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
};

static const MmInventory kDefaultInventory =
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
    { 0x12, 0, 0, 0, 0, 0 },
    {},
    {},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
    0,
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {
        { 0x15, 0x12, 0x17, 0x14, 0x3e, 0x3e, 0x3e, 0x3e },
        { 0x15, 0x12, 0x17, 0x14, 0x3e, 0x3e, 0x3e, 0x3e },
        { 0x15, 0x12, 0x17, 0x14, 0x3e, 0x3e, 0x3e, 0x3e },
    },
};

void comboCreateSaveMM(void)
{
    int i;
    int j;
    u8 tmp;

    bzero(&gMmSave, sizeof(MmSave));

    /* Create some save data */
    gMmSave.entranceIndex = 0xd800;
    gMmSave.cutscene = 0;
    gMmSave.isFirstCycle = 1;
    gMmSave.hasTatl = 1;
    gMmSave.playerForm = 4;
    gMmSave.time = 0x3fff;
    gMmSave.playerData.health = 0x30;
    gMmSave.playerData.healthCapacity = 0x30;
    gMmSave.playerData.unk_20 = 0xff00;
    gMmSave.playerData.unk_24 = 0xff00;
    gMmSave.playerData.savedSceneNum = 8;
    gMmSave.horseData.scene = 53;
    gMmSave.horseData.pos.x = -1420;
    gMmSave.horseData.pos.y = 257;
    gMmSave.horseData.pos.z = -1285;
    gMmSave.horseData.yaw = -0x7554;

    /* Set the defaults */
    memcpy(&gMmSave.itemEquips, &kDefaultItemEquips, sizeof(gMmSave.itemEquips));
    memcpy(&gMmSave.inventory, &kDefaultInventory, sizeof(gMmSave.inventory));

    /* Set the magic */
    memcpy(gMmSave.playerData.newf, "ZELDA3", 6);

    /* Copy the player name */
    copyName(gMmSave.playerData.playerName, gSave.playerName);

    /* Set some events */
    MM_SET_EVENT_WEEK(EV_MM_WEEK_ENTERED_TERMINA);
    MM_SET_EVENT_WEEK(EV_MM_WEEK_ENTERED_CLOCK_TOWN_EAST);
    MM_SET_EVENT_WEEK(EV_MM_WEEK_ENTERED_CLOCK_TOWN_NORTH);
    MM_SET_EVENT_WEEK(EV_MM_WEEK_ENTERED_CLOCK_TOWN_WEST);
    MM_SET_EVENT_WEEK(EV_MM_WEEK_TOWN_GUARDS);
    MM_SET_EVENT_WEEK(EV_MM_WEEK_FIRST_CYCLE);
    MM_SET_EVENT_WEEK(EV_MM_WEEK_FOREST_WITCH);

    /* Set the high scores */
    gMmSave.dekuPlaygroundHighScores[0] = 7500;
    gMmSave.dekuPlaygroundHighScores[1] = 7500;
    gMmSave.dekuPlaygroundHighScores[2] = 7600;
    gMmSave.unk_ee8 = 0x13000a;
    gMmSave.horseBackBalloonHighScore = 6000;
    gMmSave.shootingGalleryHighScore = 0xa0027;

    /* Set the lottery codes */
    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 3; ++j)
            gMmSave.lotteryCodes[i][j] = RandIntRange(0, 10);
    }

    /* Set the bomber code */
    gMmSave.bomberCode[0] = 1;
    gMmSave.bomberCode[1] = 2;
    gMmSave.bomberCode[2] = 3;
    gMmSave.bomberCode[3] = 4;
    gMmSave.bomberCode[4] = 5;

    for (i = 0; i < 5; ++i)
    {
        j = RandIntRange(0, 5);
        tmp = gMmSave.bomberCode[i];
        gMmSave.bomberCode[i] = gMmSave.bomberCode[j];
        gMmSave.bomberCode[j] = tmp;
    }

    /* Set the spider house masks order */
    tmp = 0xff;
    for (i = 0; i < 6; ++i)
    {
        do
        {
            gMmSave.spiderHouseMaskOrder[i] = RandIntRange(0, 0x10) & 3;
        }
        while (gMmSave.spiderHouseMaskOrder[i] == tmp);
        tmp = gMmSave.spiderHouseMaskOrder[i];
    }
}
