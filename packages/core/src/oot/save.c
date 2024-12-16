#include <combo.h>
#include <combo/item.h>
#include <combo/net.h>
#include <combo/dungeon.h>
#include <combo/dma.h>
#include <combo/time.h>
#include <combo/config.h>
#include <combo/context.h>
#include <combo/entrance.h>

static void fixSpawn(void)
{
    u32 entrance;
    s32 override;

    gSaveContext.respawnFlag = 0;
    gSaveContext.respawn[RESPAWN_MODE_DOWN].entrance = 0xffff;

    if (gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE].playerParams && !gComboCtx.valid && gPlay)
    {
        if (gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE].data & 0x80)
        {
            gComboCtx.isDungeonEntranceSpawn = 1;
            comboGameSwitch(NULL, gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE].entrance);
        }
        else
        {
            memcpy(&gSaveContext.respawn[RESPAWN_MODE_RETURN], &gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE], sizeof(OotRespawnData));
            /* Copy to the void respawn */
            memcpy(&gSaveContext.respawn[RESPAWN_MODE_DOWN], &gSaveContext.respawn[RESPAWN_MODE_RETURN], sizeof(OotRespawnData));
            gSaveContext.respawnFlag = 2;
            gSave.entrance = gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE].entrance;
            return;
        }
    }

    /* If the player saved in link's house, and it's not ER, honor that */
    if (gSave.info.sceneId == SCE_OOT_LINK_HOUSE && !Config_Flag(CFG_ER_ANY))
    {
        gSave.entrance = ENTR_OOT_SPAWN_CHILD;
        return;
    }

    entrance = gSave.age == AGE_CHILD ? ENTR_OOT_SPAWN_CHILD : ENTR_OOT_SPAWN_ADULT;
    override = comboEntranceOverride(entrance);
    if (override != -1)
        entrance = (u32)override;
    gSave.entrance = entrance;
}

void Sram_AfterOpenSave(void)
{
#if defined(DEBUG) && defined(DEBUG_OOT_ENTRANCE)
    gSave.entrance = DEBUG_OOT_ENTRANCE;
#endif

    /* Load options */
    Save_LoadOptions();

    if (Config_Flag(CFG_ONLY_MM))
        comboGameSwitch(NULL, ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER);

    /* Read the other save */
    Save_ReadForeign();

    /* Met deku tree - deku tree open - met mido */
    gSave.info.eventsChk[0] |= 0x102c;
    gSave.info.eventsMisc[0] |= 0x000b;

    if (Config_Flag(CFG_OOT_OPEN_DEKU))
        SetEventChk(EV_OOT_CHK_DEKU_MIDO_SWORD_SHIELD);

    Save_OnLoad();
    fixSpawn();
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
        comboDmaLoadFilePartial(gStartingItemsBuffer, COMBO_VROM_STARTING_ITEMS, slice * sizeof(gStartingItemsBuffer), sizeof(gStartingItemsBuffer));
        slice++;
        for (int i = 0; i < ARRAY_COUNT(gStartingItemsBuffer); i += 2)
        {
            gi = gStartingItemsBuffer[i];
            count = gStartingItemsBuffer[i + 1];
            if (gi == 0xffff)
                return;
            for (u16 j = 0; j < count; ++j)
            {
                gi = comboProgressive(gi, 0);
                comboAddItemRaw(NULL, gi);
            }
        }
    }
}

static void applyStartingEvents(void)
{
    int skippedTrials;

    /* Always set */
    SetEventChk(EV_OOT_CHK_ZELDA_FLED);
    SetEventChk(EV_OOT_CHK_ZELDA_FLED_BRIDGE);

    if (Config_Flag(CFG_DOOR_OF_TIME_OPEN))
    {
        SetEventChk(EV_OOT_CHK_DOOR_TIME);
    }

    if (Config_Flag(CFG_OOT_SKIP_ZELDA))
    {
        SetEventChk(EV_OOT_CHK_ZELDA_LETTER);
        SetEventChk(EV_OOT_CHK_SONG_ZELDA);
        SetEventChk(EV_OOT_CHK_CHILD_TALON_WOKEN);
        SetEventChk(EV_OOT_CHK_CHILD_TALON_FLED);
    }

    if (Config_Flag(CFG_OOT_OPEN_KAKARIKO_GATE))
    {
        BITMAP16_SET(gSave.info.eventsMisc, EV_OOT_INF_KAKARIKO_GATE_OPEN);
    }

    if (Config_Flag(CFG_OOT_KZ_OPEN))
    {
        SetEventChk(EV_OOT_CHK_KING_ZORA_MOVED);
    }

    if (Config_IsMq(MQ_TEMPLE_SHADOW))
    {
        gSave.info.perm[SCE_OOT_TEMPLE_SHADOW].switches |= (1 << 7);
    }

    /* Carpenters */
    if (Config_Flag(CFG_OOT_CARPENTERS_NONE) || Config_Flag(CFG_OOT_CARPENTERS_ONE))
    {
        SetEventChk(EV_OOT_CHK_CARPENTER_2);
        SetEventChk(EV_OOT_CHK_CARPENTER_3);
        SetEventChk(EV_OOT_CHK_CARPENTER_4);

        gSave.info.perm[SCE_OOT_THIEVES_HIDEOUT].switches |= (1 << 2);
        gSave.info.perm[SCE_OOT_THIEVES_HIDEOUT].switches |= (1 << 3);
        gSave.info.perm[SCE_OOT_THIEVES_HIDEOUT].switches |= (1 << 4);

        if (Config_Flag(CFG_OOT_CARPENTERS_NONE))
        {
            SetEventChk(EV_OOT_CHK_CARPENTER_1);
            gSave.info.perm[SCE_OOT_THIEVES_HIDEOUT].switches |= (1 << 1);
        }
    }

    /* Ganon trials */
    skippedTrials = 0;
    if (!Config_Flag(CFG_OOT_TRIAL_LIGHT))
    {
        SetEventChk(EV_OOT_CHK_TRIAL_LIGHT);
        skippedTrials++;
    }

    if (!Config_Flag(CFG_OOT_TRIAL_FOREST))
    {
        SetEventChk(EV_OOT_CHK_TRIAL_FOREST);
        skippedTrials++;
    }

    if (!Config_Flag(CFG_OOT_TRIAL_FIRE))
    {
        SetEventChk(EV_OOT_CHK_TRIAL_FIRE);
        skippedTrials++;
    }

    if (!Config_Flag(CFG_OOT_TRIAL_WATER))
    {
        SetEventChk(EV_OOT_CHK_TRIAL_WATER);
        skippedTrials++;
    }

    if (!Config_Flag(CFG_OOT_TRIAL_SHADOW))
    {
        SetEventChk(EV_OOT_CHK_TRIAL_SHADOW);
        skippedTrials++;
    }

    if (!Config_Flag(CFG_OOT_TRIAL_SPIRIT))
    {
        SetEventChk(EV_OOT_CHK_TRIAL_SPIRIT);
        skippedTrials++;
    }

    if (skippedTrials == 6)
        SetEventChk(EN_OOT_CHK_GANON_BARRIER);

    if (Config_Flag(CFG_OOT_FREE_SCARECROW))
    {
        SetEventChk(EV_OOT_CHK_SONG_SCARECROW_ADULT);
    }

    if (Config_Flag(CFG_MM_REMOVED_FAIRIES))
    {
        gMmSave.info.permanentSceneFlags[SCE_MM_TEMPLE_WOODFALL].switch1 |= 0x0007fc00;
        gMmSave.info.permanentSceneFlags[SCE_MM_TEMPLE_WOODFALL].collectible |= 0x70000000;

        gMmSave.info.permanentSceneFlags[SCE_MM_TEMPLE_SNOWHEAD].switch1 |= 0x0007b800;
        gMmSave.info.permanentSceneFlags[SCE_MM_TEMPLE_SNOWHEAD].collectible |= 0x40000000;

        gMmSave.info.permanentSceneFlags[SCE_MM_TEMPLE_GREAT_BAY].switch1 |= 0x00078000;
        gMmSave.info.permanentSceneFlags[SCE_MM_TEMPLE_GREAT_BAY].collectible |= 0x7c000000;
    }

    if (Config_Flag(CFG_MM_PRE_ACTIVATED_OWL_GB)) gMmSave.info.playerData.owlActivationFlags |= (1 << 0);
    if (Config_Flag(CFG_MM_PRE_ACTIVATED_OWL_ZC)) gMmSave.info.playerData.owlActivationFlags |= (1 << 1);
    if (Config_Flag(CFG_MM_PRE_ACTIVATED_OWL_SH)) gMmSave.info.playerData.owlActivationFlags |= (1 << 2);
    if (Config_Flag(CFG_MM_PRE_ACTIVATED_OWL_MV)) gMmSave.info.playerData.owlActivationFlags |= (1 << 3);
    if (Config_Flag(CFG_MM_PRE_ACTIVATED_OWL_CT)) gMmSave.info.playerData.owlActivationFlags |= (1 << 4);
    if (Config_Flag(CFG_MM_PRE_ACTIVATED_OWL_MR)) gMmSave.info.playerData.owlActivationFlags |= (1 << 5);
    if (Config_Flag(CFG_MM_PRE_ACTIVATED_OWL_WF)) gMmSave.info.playerData.owlActivationFlags |= (1 << 6);
    if (Config_Flag(CFG_MM_PRE_ACTIVATED_OWL_SS)) gMmSave.info.playerData.owlActivationFlags |= (1 << 7);
    if (Config_Flag(CFG_MM_PRE_ACTIVATED_OWL_IC)) gMmSave.info.playerData.owlActivationFlags |= (1 << 8);
    if (Config_Flag(CFG_MM_PRE_ACTIVATED_OWL_ST)) gMmSave.info.playerData.owlActivationFlags |= (1 << 9);

    BITMAP16_SET(gSave.info.eventsMisc, EV_OOT_INF_OWL_LAKE_HYLIA_SPOKEN);
    BITMAP16_SET(gSave.info.eventsMisc, EV_OOT_INF_MALON_RANCH_SAVED);

    if (Config_Flag(CFG_MM_WELL_OPEN))
    {
        gMmSave.info.permanentSceneFlags[SCE_MM_BENEATH_THE_WELL].switch0 |= 0xc0000082;
        gMmSave.info.permanentSceneFlags[SCE_MM_BENEATH_THE_WELL].switch1 |= 0x000007ff;
    }
}

void comboCreateSave(void* unk, void* buffer)
{
    int dungeonClearFlags;
    u32 base;

    /* Create MM save */
    Save_CreateMM();

    /* Move epona in a dummy scene */
    gSave.info.horseData.sceneId = -1;

    /* Apply some early settings */
    if (!Config_Flag(CFG_CHILD_WALLET))
    {
        gOotExtraFlags.childWallet = 1;
        gMmExtraFlags2.childWallet = 1;
    }
    comboWalletRefresh();

    /* Apply enemy souls */
    if (!Config_Flag(CFG_OOT_SOULS_ENEMY))
        memset(gSharedCustomSave.soulsEnemyOot, 0xff, sizeof(gSharedCustomSave.soulsEnemyOot));
    if (!Config_Flag(CFG_MM_SOULS_ENEMY))
        memset(gSharedCustomSave.soulsEnemyMm, 0xff, sizeof(gSharedCustomSave.soulsEnemyMm));
    if (!Config_Flag(CFG_OOT_SOULS_BOSS))
        memset(gSharedCustomSave.soulsBossOot, 0xff, sizeof(gSharedCustomSave.soulsBossOot));
    if (!Config_Flag(CFG_MM_SOULS_BOSS))
        memset(gSharedCustomSave.soulsBossMm, 0xff, sizeof(gSharedCustomSave.soulsBossMm));
    if (!Config_Flag(CFG_OOT_SOULS_NPC))
        memset(gSharedCustomSave.soulsNpcOot, 0xff, sizeof(gSharedCustomSave.soulsNpcOot));
    if (!Config_Flag(CFG_MM_SOULS_NPC))
        memset(gSharedCustomSave.soulsNpcMm, 0xff, sizeof(gSharedCustomSave.soulsNpcMm));
    if (!Config_Flag(CFG_OOT_SOULS_MISC))
        memset(gSharedCustomSave.soulsMiscOot, 0xff, sizeof(gSharedCustomSave.soulsMiscOot));
    if (!Config_Flag(CFG_MM_SOULS_MISC))
        memset(gSharedCustomSave.soulsMiscMm, 0xff, sizeof(gSharedCustomSave.soulsMiscMm));

    /* Apply ocarina buttons */
    if (!Config_Flag(CFG_OOT_OCARINA_BUTTONS))
        gSharedCustomSave.ocarinaButtonMaskOot = 0xffff;
    if (!Config_Flag(CFG_MM_OCARINA_BUTTONS))
        gSharedCustomSave.ocarinaButtonMaskMm = 0xffff;

    if (Config_Flag(CFG_MM_CLOCKS))
    {
        if (Config_Flag(CFG_MM_CLOCKS_PROGRESSIVE_REVERSE))
            gSharedCustomSave.mm.halfDays = 0x20;
        else if (Config_Flag(CFG_MM_CLOCKS_PROGRESSIVE))
            gSharedCustomSave.mm.halfDays = 0x01;
    }
    else
        gSharedCustomSave.mm.halfDays = 0x3f;

    /* Set initial equips */
    gOotSave.info.childEquips.buttonItems[0] = ITEM_NONE;
    gOotSave.info.childEquips.equipment.boots = 0x01;
    gOotSave.info.childEquips.equipment.tunics = 0x01;

    gOotSave.info.adultEquips.buttonItems[0] = ITEM_NONE;
    gOotSave.info.adultEquips.equipment.boots = 0x01;
    gOotSave.info.adultEquips.equipment.tunics = 0x01;

    /* Apply starting age */
    if (Config_Flag(CFG_OOT_START_ADULT))
    {
        /* Spawn as adult in ToT */
        gOotSave.age = AGE_ADULT;
        gOotSave.entrance = ENTR_OOT_WARP_SONG_TEMPLE;
        gOotSave.info.sceneId = SCE_OOT_TEMPLE_OF_TIME;

        if (Config_Flag(CFG_OOT_MUST_START_WITH_MS))
        {
            gOotSave.info.adultEquips.buttonItems[0] = ITEM_OOT_SWORD_MASTER;
            gOotSave.info.adultEquips.equipment.swords = 0x02;
            gOotSave.info.inventory.equipment.swords |= EQ_OOT_SWORD_MASTER;
            gSharedCustomSave.foundMasterSword = 1;
            gSave.info.eventsMisc[29] = 0;
        }
    }

    /* Apply starting items */
    applyStartingItems();

    /* Apply starting events */
    applyStartingEvents();

    /* Apply pre-completed dungeons */
    dungeonClearFlags = DUNGEONCLEARFLAG_BOSS;
    if (Config_Flag(CFG_REGION_STATE_DUNGEONS))
        dungeonClearFlags |= DUNGEONCLEARFLAG_WISP;
    for (int i = 0; i < 32; ++i)
    {
        if (gComboConfig.preCompleted & (1 << i))
            comboDungeonSetFlags(i, dungeonClearFlags);
    }

    /* Write save */
    Save_Write();

    /* Copy inside the buffer */
    base = 0x20 + 0x1450 * gSaveContext.fileNum;
    memcpy((char*)buffer + base, &gOotSave, 0x1354);
    memcpy((char*)buffer + base + 0x3cf0, &gOotSave, 0x1354);
}

static void DeathWarpWrapper(PlayState* play)
{
    DeathWarp(play);

    RespawnData* dungeonEntranceRespawn = &gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE];
    if (dungeonEntranceRespawn->playerParams)
    {
        if (dungeonEntranceRespawn->data & 0x80)
        {
            play->nextEntranceIndex = ENTR_CROSS_RESPAWN;
        }
        else
        {
            memcpy(&gSaveContext.respawn[RESPAWN_MODE_RETURN], dungeonEntranceRespawn, sizeof(OotRespawnData));
            play->nextEntranceIndex = dungeonEntranceRespawn->entrance;
            gSaveContext.respawnFlag = 2;
        }
    }
    else if (play->gameOverCtx.state == 5)
    {
        gSaveContext.respawnFlag = -2;
    }
}

PATCH_CALL(0x8009dacc, DeathWarpWrapper);

void PrepareAndSave(void)
{
    Save_DoSave(gPlay, 0);
}

void Save_DoSave(PlayState* play, int saveFlags)
{
    /* Wait for net */
    netWaitSave();

    gComboCtx.saveIndex = gSaveContext.fileNum;
    if (!(saveFlags & SF_PASSIVE))
    {
        if (!(saveFlags & SF_NOCOMMIT))
            PlayStoreFlags(play);
        gSave.info.sceneId = play->sceneId;
    }

    if (!(saveFlags & SF_NOCOMMIT))
    {
        Save_Write();
    }
}

static u8 sHoldTarget;
EXPORT_SYMBOL(HOLD_TARGET, sHoldTarget);

static void Save_CopySettings(void* dst, const void* src, size_t size)
{
    memcpy(dst, src, size);

    if (sHoldTarget)
    {
        ((u8*)dst)[1] = 1;
    }
}

PATCH_CALL(0x80091258, Save_CopySettings);
