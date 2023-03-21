#include <combo.h>

#define ENTRANCE_MARKET       0x1d1

static const s32 kDungeonEntrances[] = {
    0x000,
    0x004,
    0x028,
    0x169,
    0x165,
    0x010,
    0x037,
    0x082,
};

static void dungeonRespawn(s16 sceneId)
{
    int bossId;

    switch (sceneId)
    {
    case SCE_OOT_LAIR_GOHMA:
        bossId = BOSSID_GOHMA;
        break;
    case SCE_OOT_LAIR_KING_DODONGO:
        bossId = BOSSID_KING_DODONGO;
        break;
    case SCE_OOT_LAIR_BARINADE:
        bossId = BOSSID_BARINADE;
        break;
    case SCE_OOT_LAIR_PHANTOM_GANON:
        bossId = BOSSID_PHANTOM_GANON;
        break;
    case SCE_OOT_LAIR_VOLVAGIA:
        bossId = BOSSID_VOLVAGIA;
        break;
    case SCE_OOT_LAIR_MORPHA:
        bossId = BOSSID_MORPHA;
        break;
    case SCE_OOT_LAIR_BONGO_BONGO:
        bossId = BOSSID_BONGO_BONGO;
        break;
    case SCE_OOT_LAIR_TWINROVA:
        bossId = BOSSID_TWINROVA;
        break;
    default:
        return;
    }

    bossId = gComboData.boss[bossId];
    if (bossId >= BOSSID_ODOLWA)
    {
        /* Coming from MM */
        gSave.entrance = ENTRANCE_MARKET;
        return;
    }

    gSave.entrance = kDungeonEntrances[bossId];
}

void Sram_AfterOpenSave(void)
{
#if defined(DEBUG) && defined(DEBUG_OOT_ENTRANCE)
    gSave.entrance = DEBUG_OOT_ENTRANCE;
#endif

    /* Dungeon shuffle override */
    dungeonRespawn(gSave.sceneId);

    /* Game switch override */
    if (gComboCtx.valid)
    {
        gSave.entrance = ENTRANCE_MARKET;
        gComboCtx.valid = 0;

        if (gComboCtx.entrance != -1)
        {
            gSave.entrance = gComboCtx.entrance;
            gComboCtx.entrance = -1;
        }
    }
}

void Sram_CopySaveWrapper(void* fileSelect, void* sramCtx)
{
    Sram_CopySave(fileSelect, sramCtx);
    comboCopyMmSave(*(short*)((char*)fileSelect + 0x1ca50), *(short*)((char*)fileSelect + 0x1ca38));
}

PATCH_CALL(0x808048d8, Sram_CopySaveWrapper);

ALIGNED(16) static u16 gStartingItemsBuffer[64];

static void applyStartingItems(void)
{
    int slice;
    u16 gi;
    u16 count;

    slice = 0;
    for (;;)
    {
        DMARomToRam((0x03feb000 + slice * sizeof(gStartingItemsBuffer)) | PI_DOM1_ADDR2, gStartingItemsBuffer, sizeof(gStartingItemsBuffer));
        slice++;
        for (int i = 0; i < ARRAY_SIZE(gStartingItemsBuffer); i += 2)
        {
            gi = gStartingItemsBuffer[i];
            count = gStartingItemsBuffer[i + 1];
            if (gi == 0xffff)
                return;
            for (u16 j = 0; j < count; ++j)
            {
                gi = comboProgressive(gi);
                comboAddItemNoEffect(gi);
            }
        }
    }
}

static void applyStartingEvents(void)
{
    if (comboConfig(CFG_DOOR_OF_TIME_OPEN))
    {
        SetEventChk(EV_OOT_CHK_DOOR_TIME);
    }

    if (gComboData.mq & (1 << MQ_TEMPLE_SHADOW))
    {
        gSave.perm[SCE_OOT_TEMPLE_SHADOW].switches |= (1 << 7);
    }
}

void comboCreateSave(void* unk, void* buffer)
{
    u32 base;

    /* Create MM save */
    comboCreateSaveMM();

    /* Apply starting items */
    applyStartingItems();

    /* Apply starting events */
    applyStartingEvents();

    /* Write save */
    comboWriteSave();

    /* Copy inside the buffer */
    base = 0x20 + 0x1450 * gSaveContext.fileIndex;
    memcpy((char*)buffer + base, &gOotSave, 0x1354);
    memcpy((char*)buffer + base + 0x3cf0, &gOotSave, 0x1354);
}

static void DeathWarpWrapper(GameState_Play* play)
{
    dungeonRespawn(play->sceneId);
    DeathWarp(play);
}

PATCH_CALL(0x8009dacc, DeathWarpWrapper);
