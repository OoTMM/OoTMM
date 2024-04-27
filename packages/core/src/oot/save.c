#include <combo.h>
#include <combo/item.h>
#include <combo/net.h>
#include <combo/dungeon.h>
#include <combo/dma.h>
#include <combo/time.h>
#include <combo/config.h>
#include <combo/context.h>

static const u16 kDungeonEntrances[] = {
    ENTR_OOT_DEKU_TREE,
    ENTR_OOT_DODONGO_CAVERN,
    ENTR_OOT_JABU_JABU,
    ENTR_OOT_TEMPLE_FOREST,
    ENTR_OOT_TEMPLE_FIRE,
    ENTR_OOT_TEMPLE_WATER,
    ENTR_OOT_TEMPLE_SHADOW,
    ENTR_OOT_TEMPLE_SPIRIT,
};

static const u16 kBossEntrances[] = {
    ENTR_OOT_BOSS_DEKU_TREE,
    ENTR_OOT_BOSS_DODONGO_CAVERN,
    ENTR_OOT_BOSS_JABU_JABU,
    ENTR_OOT_BOSS_TEMPLE_FOREST,
    ENTR_OOT_BOSS_TEMPLE_FIRE,
    ENTR_OOT_BOSS_TEMPLE_WATER,
    ENTR_OOT_BOSS_TEMPLE_SHADOW,
    ENTR_OOT_BOSS_TEMPLE_SPIRIT,
};

static int dungeonRespawn(s16 sceneId, int isSave)
{
    int bossId;
    int dungeonId;

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
        return 0;
    }

    dungeonId = gComboConfig.boss[bossId];
    if (dungeonId >= DUNGEONID_TEMPLE_WOODFALL)
    {
        if (isSave)
            return 0;
        gSave.entrance = kBossEntrances[bossId];
    }
    else
        gSave.entrance = kDungeonEntrances[dungeonId];
    return 1;
}

static void fixSpawn(void)
{
    u32 entrance;
    s32 override;

    /* If the player saved in a boss, don't touch it */
    if (dungeonRespawn(gSave.sceneId, 1))
        return;

    /* If the player saved in a dungeon, don't touch it */
    switch (gSave.sceneId)
    {
    case SCE_OOT_INSIDE_DEKU_TREE:
    case SCE_OOT_DODONGO_CAVERN:
    case SCE_OOT_INSIDE_JABU_JABU:
    case SCE_OOT_TEMPLE_FOREST:
    case SCE_OOT_TEMPLE_FIRE:
    case SCE_OOT_TEMPLE_WATER:
    case SCE_OOT_TEMPLE_SHADOW:
    case SCE_OOT_TEMPLE_SPIRIT:
    case SCE_OOT_BOTTOM_OF_THE_WELL:
    case SCE_OOT_ICE_CAVERN:
    case SCE_OOT_GERUDO_TRAINING_GROUND:
    case SCE_OOT_INSIDE_GANON_CASTLE:
    case SCE_OOT_GANON_TOWER:
        return;
    }

    /* If the player saved in link's house, and it's not ER, honor that */
    if (gSave.sceneId == SCE_OOT_LINK_HOUSE && !Config_Flag(CFG_ER_ANY))
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
    if (Config_Flag(CFG_ONLY_MM))
        comboGameSwitch(NULL, ENTR_MM_CLOCK_TOWN);

#if defined(DEBUG) && defined(DEBUG_OOT_ENTRANCE)
    gSave.entrance = DEBUG_OOT_ENTRANCE;
#endif

    /* Read the other save */
    Save_ReadForeign();

    /* Met deku tree - deku tree open - met mido */
    gSave.eventsChk[0] |= 0x102c;
    gSave.eventsMisc[0] |= 0x000b;

    if (Config_Flag(CFG_OOT_OPEN_DEKU))
        SetEventChk(EV_OOT_CHK_DEKU_MIDO_SWORD_SHIELD);

    Save_OnLoad();
    fixSpawn();
}

void Sram_CopySaveWrapper(void* fileSelect, void* sramCtx)
{
    Sram_CopySave(fileSelect, sramCtx);
    Save_CopyMM(*(short*)((char*)fileSelect + 0x1ca50), *(short*)((char*)fileSelect + 0x1ca38));
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
        comboDmaLoadFilePartial(gStartingItemsBuffer, COMBO_VROM_STARTING_ITEMS, slice * sizeof(gStartingItemsBuffer), sizeof(gStartingItemsBuffer));
        slice++;
        for (int i = 0; i < ARRAY_SIZE(gStartingItemsBuffer); i += 2)
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
        BITMAP16_SET(gSave.eventsMisc, EV_OOT_INF_KAKARIKO_GATE_OPEN);
    }

    if (Config_Flag(CFG_OOT_KZ_OPEN))
    {
        SetEventChk(EV_OOT_CHK_KING_ZORA_MOVED);
    }

    if (gComboConfig.mq & (1 << MQ_TEMPLE_SHADOW))
    {
        gSave.perm[SCE_OOT_TEMPLE_SHADOW].switches |= (1 << 7);
    }

    /* Carpenters */
    if (Config_Flag(CFG_OOT_CARPENTERS_NONE) || Config_Flag(CFG_OOT_CARPENTERS_ONE))
    {
        SetEventChk(EV_OOT_CHK_CARPENTER_2);
        SetEventChk(EV_OOT_CHK_CARPENTER_3);
        SetEventChk(EV_OOT_CHK_CARPENTER_4);

        gSave.perm[SCE_OOT_THIEVES_HIDEOUT].switches |= (1 << 2);
        gSave.perm[SCE_OOT_THIEVES_HIDEOUT].switches |= (1 << 3);
        gSave.perm[SCE_OOT_THIEVES_HIDEOUT].switches |= (1 << 4);

        if (Config_Flag(CFG_OOT_CARPENTERS_NONE))
        {
            SetEventChk(EV_OOT_CHK_CARPENTER_1);
            gSave.perm[SCE_OOT_THIEVES_HIDEOUT].switches |= (1 << 1);
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
        gMmSave.permanentSceneFlags[SCE_MM_TEMPLE_WOODFALL].switch1 |= 0x0007fc00;
        gMmSave.permanentSceneFlags[SCE_MM_TEMPLE_WOODFALL].collectible |= 0x70000000;

        gMmSave.permanentSceneFlags[SCE_MM_TEMPLE_SNOWHEAD].switch1 |= 0x0007b800;
        gMmSave.permanentSceneFlags[SCE_MM_TEMPLE_SNOWHEAD].collectible |= 0x40000000;

        gMmSave.permanentSceneFlags[SCE_MM_TEMPLE_GREAT_BAY].switch1 |= 0x00078000;
        gMmSave.permanentSceneFlags[SCE_MM_TEMPLE_GREAT_BAY].collectible |= 0x7c000000;
    }
}

void comboCreateSave(void* unk, void* buffer)
{
    u32 base;

    /* Create MM save */
    Save_CreateMM();

    /* Move epona in a dummy scene */
    gSave.horseData.sceneId = -1;

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
    gOotSave.childEquips.buttonItems[0] = ITEM_NONE;
    gOotSave.childEquips.equipment.boots = 0x01;
    gOotSave.childEquips.equipment.tunics = 0x01;

    gOotSave.adultEquips.equipment.boots = 0x01;
    gOotSave.adultEquips.equipment.tunics = 0x01;

    if (!Config_Flag(CFG_OOT_SWORDLESS_ADULT))
    {
        gOotSave.adultEquips.buttonItems[0] = ITEM_OOT_SWORD_MASTER;
        gOotSave.adultEquips.equipment.swords = 0x02;
    }
    else
        gOotSave.adultEquips.buttonItems[0] = ITEM_NONE;

    /* Apply starting age */
    if (Config_Flag(CFG_OOT_START_ADULT))
    {
        /* Spawn as adult in ToT */
        gOotSave.age = AGE_ADULT;
        gOotSave.entrance = ENTR_OOT_WARP_SONG_TEMPLE;
        gOotSave.sceneId = SCE_OOT_TEMPLE_OF_TIME;

        if (!Config_Flag(CFG_OOT_SWORDLESS_ADULT))
        {
            /* Force Master Sword */
            gOotSave.equips.buttonItems[0] = ITEM_OOT_SWORD_MASTER;
            gOotSave.inventory.equipment.swords |= EQ_OOT_SWORD_MASTER;
            gOotSave.equips.equipment.swords = 2;
            gSharedCustomSave.foundMasterSword = 1;

            /* Unset the swordless flag */
            gSave.eventsMisc[29] = 0;
        }
    }

    /* Apply starting items */
    applyStartingItems();

    /* Apply starting events */
    applyStartingEvents();

    /* Apply pre-completed dungeons */
    for (int i = 0; i < 32; ++i)
    {
        if (gComboConfig.preCompleted & (1 << i))
            comboDungeonSetFlags(i, 0);
    }

    /* Write save */
    Save_Write();

    /* Copy inside the buffer */
    base = 0x20 + 0x1450 * gSaveContext.fileIndex;
    memcpy((char*)buffer + base, &gOotSave, 0x1354);
    memcpy((char*)buffer + base + 0x3cf0, &gOotSave, 0x1354);
}

static void DeathWarpWrapper(GameState_Play* play)
{
    dungeonRespawn(play->sceneId, 0);
    DeathWarp(play);
}

PATCH_CALL(0x8009dacc, DeathWarpWrapper);

void PrepareAndSave(void)
{
    Save_DoSave(gPlay, 0);
}

void Save_DoSave(GameState_Play* play, int saveFlags)
{
    /* Wait for net */
    netWaitSave();

    gComboCtx.saveIndex = gSaveContext.fileIndex;
    if (!(saveFlags & SF_PASSIVE))
    {
        if (!(saveFlags & SF_NOCOMMIT))
            PlayStoreFlags(play);
        gSave.sceneId = play->sceneId;
    }

    if (!(saveFlags & SF_NOCOMMIT))
    {
        Save_Write();
    }
}
