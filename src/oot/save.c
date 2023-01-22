#include <combo.h>

#define ENTRANCE_MARKET       0x1d1

void Sram_AfterOpenSave(void)
{
#if defined(DEBUG) && defined(DEBUG_OOT_ENTRANCE)
    gSave.entrance = DEBUG_OOT_ENTRANCE;
#endif

    if (gComboCtx.valid)
    {
        gSave.entrance = ENTRANCE_MARKET;
        gComboCtx.valid = 0;
    }
}

void Sram_CopySaveWrapper(void* fileSelect, void* sramCtx)
{
    Sram_CopySave(fileSelect, sramCtx);
    comboCopyMmSave(*(short*)((char*)fileSelect + 0x1ca50), *(short*)((char*)fileSelect + 0x1ca38));
}

PATCH_CALL(0x808048d8, Sram_CopySaveWrapper);

static u32 kDungeonMapCompassMaskOot = 0x3ff;

static void startingMapCompass(void)
{
    for (int i = 0; i < SCE_OOT_GANON_TOWER_COLLAPSING; ++i)
    {
        if ((1 << i) & kDungeonMapCompassMaskOot)
        {
            gOotSave.dungeonItems[i].map = 1;
            gOotSave.dungeonItems[i].compass = 1;
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        gMmSave.inventory.dungeonItems[i].map = 1;
        gMmSave.inventory.dungeonItems[i].compass = 1;
    }
}

ALIGNED(16) static u16 gStartingItemsBuffer[64];

static void applyStartingItems(void)
{
    int slice;
    u16 gi;
    u16 count;

    slice = 0;
    for (;;)
    {
        DMARomToRam((0x03fe7000 + slice * sizeof(gStartingItemsBuffer)) | PI_DOM1_ADDR2, gStartingItemsBuffer, sizeof(gStartingItemsBuffer));
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

void comboCreateSave(void* unk, void* buffer)
{
    u32 base;

    /* Create MM save */
    comboCreateSaveMM();

    if (comboConfig(CFG_STARTING_MAP_COMPASS))
        startingMapCompass();

    /* Apply starting items */
    applyStartingItems();

    /* Write save */
    comboWriteSave();

    /* Copy inside the buffer */
    base = 0x20 + 0x1450 * gSaveContext.fileIndex;
    memcpy((char*)buffer + base, &gOotSave, 0x1354);
    memcpy((char*)buffer + base + 0x3cf0, &gOotSave, 0x1354);
}
