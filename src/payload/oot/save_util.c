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
        { 0xff, 0xff, 0xff, 0xff },
        { 0xff, 0xff, 0xff, 0xff },
        { 0xfd, 0xff, 0xff, 0xff },
    },
    {
        { 0x00, 0xff, 0xff, 0xff },
        { 0xff, 0xff, 0xff, 0xff },
        { 0xff, 0xff, 0xff, 0xff },
        { 0xff, 0xff, 0xff, 0xff },
    },
    0x0,
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
    0x120000,
    0,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    0,
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {
        { 0x15, 0x12, 0x17, 0x14, 0x3e, 0x3e, 0x3e, 0x3e },
        { 0x15, 0x12, 0x17, 0x14, 0x3e, 0x3e, 0x3e, 0x3e },
        { 0x15, 0x12, 0x17, 0x14, 0x3e, 0x3e, 0x3e, 0x3e },
    },
    0
};

void zeroComboSaveData(void)
{
    char buffer[0x800];
    bzero(buffer, sizeof(buffer));
    for (int i = 0; i < 8; ++i)
    {
        comboReadWriteFlash(0x30000 + 0x800 * i + 0x4000 * gSaveContext.fileIndex, buffer, sizeof(buffer), OS_WRITE);
    }
}

void comboCreateSaveMM(void)
{
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

    /* Also zero the combo save */
    zeroComboSaveData();

    /* Save */
    comboWriteForeignSave();
}
