#include <combo.h>
#include <combo/net.h>
#include <combo/config.h>
#include <combo/inventory.h>
#include <combo/entrance.h>

void Sram_AfterOpenSave(void)
{
    /* Load options */
    Save_LoadOptions();

    /* Read the foreign save */
    Save_ReadForeign();

    /* Handle common settings */
    Save_OnLoad();

    gSave.playerForm = MM_PLAYER_FORM_HUMAN;
    gSave.equippedMask = 0;
    gSave.entrance = ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER;

#if defined(DEBUG) && defined(DEBUG_MM_ENTRANCE)
    gSave.entrance = DEBUG_MM_ENTRANCE;
#endif
}

void Sram_SaveEndOfCycle(PlayState* play)
{
    /* Re-enable time */
    gNoTimeFlow = 0;

    /* Disable sirloin */
    gSharedCustomSave.storedSirloin = 0;

    /* Store a dummy scene/entrance in OoT (prevents reloading into a temple - could lead to softlocks) */
    gOotSave.info.sceneId = SCE_OOT_TEMPLE_OF_TIME;
    gOotSave.entrance = ENTR_OOT_WARP_SONG_TEMPLE;

    Save_DoSave(play, SF_NOCOMMIT);
    _Sram_SaveEndOfCycle(play);

    /* Not an Owl save */
    gSave.isOwlSave = 0;

    /* Revive bosses */
    gMmExtraBoss.bossCycle = 0;

    /* Regain trade quest items */
    gMmExtraTrade.trade1 = gMmExtraTrade.tradeObtained1;
    gMmExtraTrade.trade2 = gMmExtraTrade.tradeObtained2;
    gMmExtraTrade.trade3 = gMmExtraTrade.tradeObtained3;

    if (gMmExtraTrade.trade1)
    {
        gSave.info.inventory.items[ITS_MM_TRADE1] = ITEM_MM_DEED_OCEAN;
        comboToggleSlot(ITS_MM_TRADE1);
    }

    if (gMmExtraTrade.trade2)
    {
        gSave.info.inventory.items[ITS_MM_TRADE2] = ITEM_MM_LETTER_TO_MAMA;
        comboToggleSlot(ITS_MM_TRADE2);
    }

    if (gMmExtraTrade.trade3)
    {
        gSave.info.inventory.items[ITS_MM_TRADE3] = ITEM_MM_PENDANT_OF_MEMORIES;
        comboToggleSlot(ITS_MM_TRADE3);
    }

    if (Config_Flag(CFG_MM_KEEP_ITEMS_RESET))
    {
        /* Don't show item loss in the cutscene */
        gSave.eventInf[7] &= 0xe0;

        /* Still reset pictobox */
        gSave.info.inventory.ammo[ITS_MM_PICTOBOX] = 0;
    }
    else
    {
        /* Reset ammo */
        for (int i = 0; i < 24; ++i)
            gSave.info.inventory.ammo[i] = 0;

        /* Reset rupees */
        gSave.info.playerData.rupees = 0;
        gSave.rupeesDelta = 0;

        /* Reset chateau */
        MM_CLEAR_EVENT_WEEK(EV_MM_WEEK_DRANK_CHATEAU_ROMANI);
    }

    /* Empty bottles (except gold dust) */
    for (int i = 0; i < 6; ++i)
    {
        u8* slot;

        slot = &gSave.info.inventory.items[ITS_MM_BOTTLE + i];
        switch (*slot)
        {
        case ITEM_NONE:
        case ITEM_MM_GOLD_DUST:
            break;
        case ITEM_MM_SPRING_WATER_HOT:
            if (!Config_Flag(CFG_MM_KEEP_ITEMS_RESET))
                *slot = ITEM_MM_BOTTLE_EMPTY;
            else
                *slot = ITEM_MM_SPRING_WATER;
            break;
        case ITEM_MM_ZORA_EGG:
        case ITEM_MM_SEAHORSE:
        case ITEM_MM_DEKU_PRINCESS:
        case ITEM_MM_MAGIC_MUSHROOM:
        case ITEM_MM_BIG_POE:
            /* Remove these items to avoid softlocks */
            /* TODO: Maybe allow C-up/L instead? */
            *slot = ITEM_MM_BOTTLE_EMPTY;
            break;
        default:
            if (!Config_Flag(CFG_MM_KEEP_ITEMS_RESET))
                *slot = ITEM_MM_BOTTLE_EMPTY;
            break;
        }
    }

    /* Reload bottles */
    for (int i = 1; i <= 3; ++i)
    {
        int slot;

        slot = gSave.info.itemEquips.cButtonSlots[0][i];
        if (slot >= ITS_MM_BOTTLE && slot <= ITS_MM_BOTTLE6)
        {
            gSave.info.itemEquips.buttonItems[0][i] = gSave.info.inventory.items[slot];
            Interface_LoadItemIconImpl(play, i);
        }
    }

    /* Reset unk_14 and room flags, except for dungeons */
    for (int i = 7; i < ARRAY_COUNT(gSave.info.permanentSceneFlags); ++i)
    {
        switch (i)
        {
        case SCE_MM_TEMPLE_WOODFALL:
        case SCE_MM_TEMPLE_SNOWHEAD:
        case SCE_MM_TEMPLE_GREAT_BAY:
        case SCE_MM_TEMPLE_STONE_TOWER:
        case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
            break;
        default:
            gSave.info.permanentSceneFlags[i].clearedFloors = 0;
            gSave.info.permanentSceneFlags[i].rooms = 0;
            break;
        }
    }

    comboHandleAutoInvertClockSpeed();
}

PATCH_CALL(0x80146f94, Sram_SaveEndOfCycle);
PATCH_CALL(0x80158420, Sram_SaveEndOfCycle);

void PrepareSaveAndSave(SramContext* sram)
{
    PrepareSave(sram);
    Save_Write();
}

PATCH_CALL(0x80146fa0, PrepareSaveAndSave);
PATCH_CALL(0x80158438, PrepareSaveAndSave);
PATCH_CALL(0x80158554, PrepareSaveAndSave);
PATCH_CALL(0x80829218, PrepareSaveAndSave);
PATCH_CALL(0x80829f08, PrepareSaveAndSave);
PATCH_CALL(0x80146f10, PrepareSaveAndSave);

void Save_DoSave(PlayState* play, int saveFlags)
{
    /* Wait for net */
    netWaitSave();

    /* Mark the save as owl */
    if (saveFlags & SF_OWL)
        gSave.isOwlSave = 1;

    /* Reset the OoT spawn point */
    gOotSave.info.sceneId = SCE_OOT_TEMPLE_OF_TIME;
    gOotSave.entrance = ENTR_OOT_WARP_SONG_TEMPLE;

    if (!(saveFlags & SF_NOCOMMIT))
    {
        PlayStoreFlags(play);
        PrepareSave(&play->sramCtx);
        Save_Write();
    }
}

static void MoonCrashReset(void)
{
    s32 cutscene;

    /* Read save */
    cutscene = gSave.cutscene;
    Save_ReadOwn();
    Save_ReadForeign();
    gSave.cutscene = cutscene;

    /* Reset flags */
    for (int i = 0; i < ARRAY_COUNT(gSaveContext.cycleSceneFlags); ++i)
    {
        gSaveContext.cycleSceneFlags[i].chest = gSave.info.permanentSceneFlags[i].chest;
        gSaveContext.cycleSceneFlags[i].collectible = gSave.info.permanentSceneFlags[i].collectible;
        gSaveContext.cycleSceneFlags[i].switch0 = gSave.info.permanentSceneFlags[i].switch0;
        gSaveContext.cycleSceneFlags[i].switch1 = gSave.info.permanentSceneFlags[i].switch1;
        gSaveContext.cycleSceneFlags[i].clearedRoom = gSave.info.permanentSceneFlags[i].clearedRoom;
    }

    /* Reset timers */
    for (int i = 0; i < TIMER_ID_MAX; ++i)
    {
        gSaveContext.timerStates[i] = 0;
        gSaveContext.timerCurTimes[i] = 0;
        gSaveContext.timerTimeLimits[i] = 0;
        gSaveContext.timerStartOsTimes[i] = 0;
        gSaveContext.timerStopTimes[i] = 0;
        gSaveContext.timerPausedOsTimes[i] = 0;
    }

    *((u32*)0x801bdaa0) = 1;
    gHorseIsMounted = 0;
    gSaveContext.powderKegTimer = 0;

    /* Trigger save load */
    Save_OnLoad();
}

static void MoonCrashCycle(void)
{
    s32 cutscene;

    /* Start a new cycle */
    cutscene = gSave.cutscene;
    gSave.playerForm = MM_PLAYER_FORM_HUMAN;
    gSave.equippedMask = 0;
    Sram_SaveEndOfCycle(gPlay);
    gSave.cutscene = cutscene;
}

void Sram_ResetSaveFromMoonCrash(void)
{
    if (Config_Flag(CFG_MM_MOON_CRASH_CYCLE))
        MoonCrashCycle();
    else
        MoonCrashReset();
}

PATCH_FUNC(0x80144a94, Sram_ResetSaveFromMoonCrash);

void DeathWarpWrapper(PlayState* play)
{
    DeathWarp(play);

    RespawnData* dungeonEntranceRespawn = &gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE];
    if (dungeonEntranceRespawn->playerParams)
    {
        if (!(dungeonEntranceRespawn->data & 0x80))
        {
            play->nextEntrance = ENTR_CROSS_RESPAWN;
        }
        else
        {
            memcpy(&gSaveContext.respawn[RESPAWN_MODE_TOP], dungeonEntranceRespawn, sizeof(RespawnData));
            gSaveContext.respawn[RESPAWN_MODE_TOP].data &= 0x7f;
            play->nextEntrance = dungeonEntranceRespawn->entrance;
        }
    }
}

PATCH_CALL(0x80169fe4, DeathWarpWrapper);
PATCH_CALL(0x801aa8b8, DeathWarpWrapper);
PATCH_CALL(0x80af23e8, DeathWarpWrapper);
