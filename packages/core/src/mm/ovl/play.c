#include <combo.h>
#include <combo/net.h>
#include <combo/menu.h>
#include <combo/entrance.h>
#include <combo/time.h>
#include <combo/custom.h>

GameState_Play* gPlay;
int gNoTimeFlow;

static int sTimeSkipFlag;

static void checkTimeSkip(GameState_Play* play)
{
    Actor_Player* link;
    int currentHalfDay;
    int nextHalfDay;
    u32 linearTime;
    u16 t;
    u8 d;

    if (gNoTimeFlow)
    {
        sTimeSkipFlag = 0;
        return;
    }

    switch (play->sceneId)
    {
    case SCE_MM_MOON:
    case SCE_MM_MOON_DEKU:
    case SCE_MM_MOON_GORON:
    case SCE_MM_MOON_ZORA:
    case SCE_MM_MOON_LINK:
    case SCE_MM_LAIR_MAJORA:
        sTimeSkipFlag = 0;
        return;
    }

    if (Player_InCsMode(play))
    {
        sTimeSkipFlag = 0;
        return;
    }

    d = gSave.day;
    t = gSave.time;

    /* Check for day 0 / day 4 */
    if (d < 1 || d >= 4)
        return;

    /* Check if we have the clock for the current half day */
    linearTime = Time_Game2Linear(d, t);
    currentHalfDay = (linearTime - 0x10000) / 0x8000;

    if (gSharedCustomSave.mm.halfDays & (1 << currentHalfDay))
    {
        sTimeSkipFlag = 0;
        return;
    }

    /* We don't, check for the next clock we have */
    nextHalfDay = -1;
    for (int i = currentHalfDay; i < 6; ++i)
    {
        if (gSharedCustomSave.mm.halfDays & (1 << i))
        {
            nextHalfDay = i;
            break;
        }
    }

    if (nextHalfDay == -1)
    {
        /* We have no clock going forward, it's a moon crash */
        Interface_StartMoonCrash(play);
    }
    else
    {
        if (play->actorCtx.flags & 2)
        {
            /* Telescope */
            AudioSeq_QueueSeqCmd(0xe0000100);
            gSaveContext.nextCutscene = 0;
            comboTransition(play, ENTR_MM_ASTRAL_OBSERVATORY_FROM_FIELD);
            gSave.time -= 0x10;
            return;
        }

        if (sTimeSkipFlag)
        {
            /* We have a clock going forward */
            Time_Linear2Game(&d, &t, Time_FromHalfDay(nextHalfDay));
            gSave.day = d;
            gSave.time = t;
            gSave.isNight = !!(nextHalfDay & 1);

            /* Need a reload */
            link = GET_LINK(play);
            Play_SetRespawnData(play, 1, gSave.entranceIndex, play->roomCtx.curRoom.id, 0xdff, &link->base.world.pos, link->base.rot2.y);
            gSaveContext.respawnFlag = 2;
            gSaveContext.nextCutscene = 0;
            comboTransition(play, gSave.entranceIndex);
        }
        else
            sTimeSkipFlag = 1;
    }
}

static u32 entranceForOverride(u32 entrance)
{
    u32 entranceKey;

    entranceKey = (entrance >> 9);
    switch (entranceKey)
    {
    case 0x06: entranceKey = 0x42; break;
    case 0x57: entranceKey = 0x4d; break;
    case 0x45: entranceKey = 0x4a; break;
    case 0x5b: entranceKey = 0x5a; break;
    }
    entrance = (entranceKey << 9) | (entrance & 0x1ff);

    switch (entrance)
    {
    case ENTR_MM_GROTTO_TYPE_GENERIC:
        switch (gGrottoData & 0x1f)
        {
        case 0x13: return ENTR_MM_GROTTO_GENERIC_PATH_SNOWHEAD;
        case 0x14: return ENTR_MM_GROTTO_GENERIC_VALLEY;
        case 0x15: return ENTR_MM_GROTTO_GENERIC_ZORA_CAPE;
        case 0x16: return ENTR_MM_GROTTO_GENERIC_PATH_IKANA;
        case 0x17: return ENTR_MM_GROTTO_GENERIC_GREAT_BAY_COAST;
        case 0x18: return ENTR_MM_GROTTO_GENERIC_GRAVEYARD;
        case 0x19: return ENTR_MM_GROTTO_GENERIC_TWIN_ISLANDS;
        case 0x1a: return ENTR_MM_GROTTO_GENERIC_FIELD_PILLAR;
        case 0x1b: return ENTR_MM_GROTTO_GENERIC_MOUNTAIN_VILLAGE;
        case 0x1c: return ENTR_MM_GROTTO_GENERIC_WOODS;
        case 0x1d: return ENTR_MM_GROTTO_GENERIC_SWAMP;
        case 0x1e: return ENTR_MM_GROTTO_GENERIC_PATH_SWAMP;
        case 0x1f: return ENTR_MM_GROTTO_GENERIC_GRASS;
        }
        UNREACHABLE();
    case ENTR_MM_GROTTO_TYPE_COW:
        switch (gLastScene)
        {
        case SCE_MM_TERMINA_FIELD: return ENTR_MM_GROTTO_COW_FIELD;
        case SCE_MM_GREAT_BAY_COAST: return ENTR_MM_GROTTO_COW_COAST;
        }
        UNREACHABLE();
    default:
        return entrance;
    }
}

static void sendSelfMajorasMask(void)
{
    NetContext* net;
    int npc;
    s16 gi;

    if (!comboConfig(CFG_MULTIPLAYER))
        return;

    gi = GI_MM_MASK_MAJORA;
    npc = NPC_MM_MAJORA;

    net = netMutexLock();
    netWaitCmdClear();
    bzero(&net->cmdOut, sizeof(net->cmdOut));
    net->cmdOut.op = NET_OP_ITEM_SEND;
    net->cmdOut.itemSend.playerFrom = gComboData.playerId;
    net->cmdOut.itemSend.playerTo = gComboData.playerId;
    net->cmdOut.itemSend.game = 1;
    net->cmdOut.itemSend.gi = gi;
    net->cmdOut.itemSend.key = ((u32)OV_NPC << 24) | npc;
    net->cmdOut.itemSend.flags = 0;
    netMutexUnlock();

    /* Mark the NPC as obtained */
    BITMAP8_SET(gSharedCustomSave.mm.npc, npc);
}

void fixupOriginalSceneSetup(void)
{
    u32 entranceKey;
    u32 entrance;

    entrance = gSave.entranceIndex;
    entranceKey = (entrance >> 9);

    /* Swamp State */
    if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_WF))
    {
        switch (entranceKey)
        {
        case 0x42: /* Swamp */
            entranceKey = 0x06;
            break;
        case 0x43: /* Woodfall */
            gSaveContext.nextCutscene = 0xfff0;
            break;
        }
    }

    /* Snowhead State */
    if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH))
    {
        switch (entranceKey)
        {
        case 0x4d: /* Mountain Village */
            entranceKey = 0x57;
            break;
        case 0x5a: /* Twin Island */
            entranceKey = 0x5b;
            break;
        case 0x4a: /* Goron Village */
            entranceKey = 0x45;
            break;
        case 0x59: /* Snowhead */
            gSaveContext.nextCutscene = 0xfff0;
            break;
        }
    }

    /* Great Bay */
    if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_GB))
    {
        switch (entranceKey)
        {
        case 0x34: /* Great Bay */
            gSaveContext.nextCutscene = 0xfff0;
            break;
        case 0x35: /* Zora Cape */
            gSaveContext.nextCutscene = 0xfff0;
            break;
        }
    }

    /* Ikana */
    if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_ST))
    {
        switch (entranceKey)
        {
        case 0x10: /* Ikana Canyon */
            gSaveContext.nextCutscene = 0xfff0;
            break;
        }
    }

    gSave.entranceIndex = (entranceKey << 9) | (entrance & 0x1ff);
}


typedef struct
{
    u16 entrance;
    u8  room;
    s16 pos[3];
}
GrottoExit;

static const GrottoExit kGrottoExits[] = {
    /* Generic grottos */
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_EAST, 0, { 2367, 315, -192 } },
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_SOUTH, 0, { 1012, -221, 3642 } },
    { ENTR_MM_SWAMP_ROAD_FROM_FIELD, 0, { 104, -182, 2202 } },
    { ENTR_MM_MYSTERY_WOODS, 2, { 2, 0, -889 } },
    { ENTR_MM_SWAMP_FROM_SPIDER_HOUSE, 1, { -1700, 38, 1800 } },
    { ENTR_MM_WARP_OWL_MOUNTAIN_VILLAGE, 1, { 2406, 1168, -1197 } },
    { ENTR_MM_TWIN_ISLANDS_FROM_MOUNTAIN_VILLAGE, 0, { -1309, 320, 143 } },
    { ENTR_MM_PATH_SNOWHEAD_FROM_SNOWHEAD, 0, { -987, 360, -2339 } },
    { ENTR_MM_GREAT_BAY_COAST_FROM_FISHER_HUT, 0, { 1359, 80, 5018 } },
    { ENTR_MM_ZORA_CAPE_FROM_GREAT_BAY_COAST, 0, { -562, 80, 2707 } },
    { ENTR_MM_IKANA_ROAD_FROM_FIELD, 0, { -428, 200, -335 } },
    { ENTR_MM_IKANA_GRAVEYARD_FROM_DAMPE, 1, { 106, 314, -1777 } },
    { ENTR_MM_IKANA_VALLEY_FROM_ROAD, 2, { -2475, -505, 2475 } },

    /* Cow grottos */
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_SOUTH, 0, { -375, -222, 3976 } },
    { ENTR_MM_GREAT_BAY_COAST_FROM_LABORATORY, 0, { 2077, 333, -215 } },

    /* Gossip grottos */
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_NORTH, 0, { 192, 48, -3138 } },
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_EAST,  0, { 4450, 254, 925 } },
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_WEST,  0, { -2782, 48, -1654 } },
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_SOUTH, 0, { -1592, -222, 4622 } },

    /* Normal grottos */
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_NORTH, 0, { -2425, -281, -3291 } },
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_EAST,  0, { 3223, 219, 1417 } },
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_SOUTH, 0, { -2317, -221, 3418 } },
    { ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_WEST,  0, { -5159, -281, -571 } },
    { ENTR_MM_TWIN_ISLANDS_FROM_MOUNTAIN_VILLAGE, 0, { 589, 195, 53 } },
};

static const GrottoExit kGrottoExitMountainWinter = { ENTR_MM_WARP_OWL_MOUNTAIN_VILLAGE, 0, { 345, 8, -150 } };

static void applyGrottoExit(u32* entrance, const GrottoExit* ge)
{
    RespawnData* rs;

    /* Set the grotto exit */
    rs = &gSaveContext.respawn[3];
    rs->pos.x = ge->pos[0];
    rs->pos.y = ge->pos[1];
    rs->pos.z = ge->pos[2];
    rs->yaw = 0;
    rs->entrance = ge->entrance;
    rs->playerParams = 0x04ff;
    rs->data = 0;
    rs->roomIndex = ge->room;
    rs->tempSwitchFlags = 0;
    rs->unk_18 = 0;
    rs->tempCollectFlags = 0;

    /* Copy to the void respawn */
    memcpy(&gSaveContext.respawn[0], rs, sizeof(RespawnData));

    /* Set the respawn flags */
    gSaveContext.respawnFlag = 4;
    *entrance = rs->entrance;
}

static u32 entrGrottoExit(GameState_Play* play)
{
    if (!comboConfig(CFG_ER_GROTTOS))
        return ENTR_MM_INTERNAL_EXIT_GROTTO;

    switch (play->sceneId)
    {
    case SCE_MM_GROTTOS:
        switch (play->roomCtx.curRoom.id)
        {
        case 0x00: return ENTR_MM_GROTTO_EXIT_GOSSIPS_OCEAN;
        case 0x01: return ENTR_MM_GROTTO_EXIT_GOSSIPS_SWAMP;
        case 0x02: return ENTR_MM_GROTTO_EXIT_GOSSIPS_CANYON;
        case 0x03: return ENTR_MM_GROTTO_EXIT_GOSSIPS_MOUNTAIN;
        case 0x04:
            switch (gGrottoData & 0x1f)
            {
            case 0x13: return ENTR_MM_GROTTO_EXIT_GENERIC_PATH_SNOWHEAD;
            case 0x14: return ENTR_MM_GROTTO_EXIT_GENERIC_VALLEY;
            case 0x15: return ENTR_MM_GROTTO_EXIT_GENERIC_ZORA_CAPE;
            case 0x16: return ENTR_MM_GROTTO_EXIT_GENERIC_PATH_IKANA;
            case 0x17: return ENTR_MM_GROTTO_EXIT_GENERIC_GREAT_BAY_COAST;
            case 0x18: return ENTR_MM_GROTTO_EXIT_GENERIC_GRAVEYARD;
            case 0x19: return ENTR_MM_GROTTO_EXIT_GENERIC_TWIN_ISLANDS;
            case 0x1a: return ENTR_MM_GROTTO_EXIT_GENERIC_FIELD_PILLAR;
            case 0x1b: return ENTR_MM_GROTTO_EXIT_GENERIC_MOUNTAIN_VILLAGE;
            case 0x1c: return ENTR_MM_GROTTO_EXIT_GENERIC_WOODS;
            case 0x1d: return ENTR_MM_GROTTO_EXIT_GENERIC_SWAMP;
            case 0x1e: return ENTR_MM_GROTTO_EXIT_GENERIC_PATH_SWAMP;
            case 0x1f: return ENTR_MM_GROTTO_EXIT_GENERIC_GRASS;
            }
            UNREACHABLE();
        case 0x07: return ENTR_MM_GROTTO_EXIT_DODONGO;
        case 0x09: return ENTR_MM_GROTTO_EXIT_SCRUB;
        case 0x0a:
            switch (gLastScene)
            {
            case SCE_MM_TERMINA_FIELD: return ENTR_MM_GROTTO_EXIT_COW_FIELD;
            case SCE_MM_GREAT_BAY_COAST: return ENTR_MM_GROTTO_EXIT_COW_COAST;
            }
            UNREACHABLE();
        case 0x0b: return ENTR_MM_GROTTO_EXIT_BIO_BABA;
        case 0x0d: return ENTR_MM_GROTTO_EXIT_PEAHAT;
        case 0x0e: return ENTR_MM_GROTTO_EXIT_HOT_WATER;
        }
    }

    return ENTR_MM_INTERNAL_EXIT_GROTTO;
}

static const u8 kGrottoDataGeneric[] = { 0x1a, 0x1f, 0x1e, 0x1c, 0x1d, 0x1b, 0x19, 0x13, 0x17, 0x15, 0x16, 0x18, 0x14 };

static void applyCustomEntrance(u32* entrance)
{
    const GrottoExit* ge;
    u32 id;

    id = *entrance;
    if (id >= ENTR_MM_GROTTO_GENERIC_FIELD_PILLAR && id <= ENTR_MM_GROTTO_GENERIC_VALLEY)
    {
        id -= ENTR_MM_GROTTO_GENERIC_FIELD_PILLAR;
        *entrance = ENTR_MM_GROTTO_TYPE_GENERIC;
        gGrottoData &= ~0x1f;
        gGrottoData |= kGrottoDataGeneric[id];
    }
    else if (id >= ENTR_MM_GROTTO_COW_FIELD && id <= ENTR_MM_GROTTO_COW_COAST)
    {
        if (id == ENTR_MM_GROTTO_COW_FIELD)
            gLastScene = SCE_MM_TERMINA_FIELD;
        else
            gLastScene = SCE_MM_GREAT_BAY_COAST;
        *entrance = ENTR_MM_GROTTO_TYPE_COW;
    }
    else if (id >= ENTR_MM_GROTTO_EXIT_GENERIC_FIELD_PILLAR && id <= ENTR_MM_GROTTO_EXIT_HOT_WATER)
    {
        if ((id == ENTR_MM_GROTTO_EXIT_GENERIC_MOUNTAIN_VILLAGE) && !MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH))
            ge = &kGrottoExitMountainWinter;
        else
        {
            id -= ENTR_MM_GROTTO_EXIT_GENERIC_FIELD_PILLAR;
            ge = &kGrottoExits[id];
        }
        applyGrottoExit(entrance, ge);
    }
}

static void spawnSirloin(GameState_Play* play)
{
    if (!gSharedCustomSave.storedSirloin)
        return;
    if (play->sceneId != SCE_MM_MOUNTAIN_VILLAGE_WINTER)
        return;
    if (MM_CHECK_EVENT_INF(EV_MM_WEEK_DUNGEON_SH))
        return;

    SpawnActor(&play->actorCtx, play, AC_EN_MM, -1025.f, 8.f, 400.f, 0.f, 0.f, 0.f, 0x8000);
}

void preInitTitleScreen(void)
{
    u32 entrance;

    if (!gComboCtx.valid)
        return;

    /* Disable Title screen */
    gSaveContext.gameMode = 0;

    /* Load save */
    gSaveContext.fileIndex = gComboCtx.saveIndex;
    Sram_OpenSave(NULL, NULL);

    if (gComboCtx.isFwSpawn)
    {
        gSaveContext.respawnFlag = 8;
        gComboCtx.isFwSpawn = 0;

        RespawnData* fw = &gCustomSave.fw[gOotSave.age];

        if (fw->data)
        {
            gSaveContext.respawn[RESPAWN_MODE_HUMAN] = *fw;
        }
        else
        {
            gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = 0;
            gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.x = 0.0f;
            gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.y = 0.0f;
            gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos.z = 0.0f;
        }
    }

    gSave.cutscene = 0;
    gSaveContext.nextCutscene = 0;

    /* Load the entrance */
    entrance = gComboCtx.entrance;
    if (comboConfig(CFG_ER_ANY))
        g.initialEntrance = entrance;
    else
        g.initialEntrance = ENTR_MM_CLOCK_TOWN;
    applyCustomEntrance(&entrance);
    gSave.entranceIndex = entrance;

    /* Fixup the scene/setup */
    fixupOriginalSceneSetup();

    /* Handle shuffled entrance */
    switch (gSave.entranceIndex)
    {
    case ENTR_MM_BOSS_TEMPLE_WOODFALL:
    case ENTR_MM_BOSS_TEMPLE_SNOWHEAD:
    case ENTR_MM_BOSS_TEMPLE_GREAT_BAY:
    case ENTR_MM_BOSS_TEMPLE_STONE_TOWER:
        gNoTimeFlow = 1;
        break;
    }

    /* Finished */
    gComboCtx.valid = 0;
}

void hookPlay_Init(GameState_Play* play)
{
    u32 entrance;

    gPlay = play;
    int isEndOfGame;

    /* Pre-init */
    gIsEntranceOverride = 0;
    preInitTitleScreen();

    /* Init */
    sTimeSkipFlag = 0;
    isEndOfGame = 0;
    gActorCustomTriggers = NULL;
    gMultiMarkChests = 0;
    gMultiMarkCollectibles = 0;
    gMultiMarkSwitch0 = 0;
    gMultiMarkSwitch1 = 0;
    g.keatonGrassMax = -1;
    comboMultiResetWisps();

    if (comboConfig(CFG_ER_OVERWORLD) || comboConfig(CFG_ER_INDOORS))
        gSave.hasSirloin = 0;

    if (gSaveContext.respawnFlag == 8)
    {
        s32 fwSceneId = Entrance_GetSceneIdAbsolute(gSaveContext.respawn[RESPAWN_MODE_HUMAN].entrance);
        Vec3f* pos = &gSaveContext.respawn[RESPAWN_MODE_HUMAN].pos;
        if (fwSceneId == SCE_MM_GORON_VILLAGE_SPRING || fwSceneId == SCE_MM_GORON_VILLAGE_WINTER)
        {
            if (MM_GET_EVENT_WEEK(EV_MM_WEEK_DUNGEON_SH) && pos->x > 1100.0f) /* from Lens Cave */
            {
                /* spawn near the owl area instead */
                pos->x = 1189.0f;
                pos->z = -911.0f;
            }
        }
        else if (fwSceneId == SCE_MM_WOODFALL)
        {
            if (!MM_GET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE) && ABS(pos->z) < 500.0f)
            {
                if (pos->z > 0) /* from front of temple */
                {
                    /* spawn at owl statue instead */
                    pos->x = 1.0f;
                    pos->y = 200.0f;
                    pos->z = 1094.0f;
                }
                else /* from back of temple */
                {
                    /* spawn near exit to southern swamp instead */
                    pos->x = -41.0f;
                    pos->y = 12.0f;
                    pos->z = -1353.0f;
                }
            }
        }
        gCustomSave.fw[gOotSave.age].pos = *pos;
    }

    if (comboIsLinkAdult())
    {
        s16* playerFormObjectIds = (s16*)0x801c2730;
        playerFormObjectIds[MM_PLAYER_FORM_HUMAN] = CUSTOM_OBJECT_ID_MM_ADULT_LINK;

        u32* playerSkeletons = (u32*)0x801bfe00;
        playerSkeletons[MM_PLAYER_FORM_HUMAN] = CUSTOM_OBJECT_MM_ADULT_LINK_0;

        u32* playerWaistDLs = (u32*)0x801bfffc;
        u32* playerHandHoldingShieldDLs = (u32*)0x801c0024;
        u32* playerSheath12DLs = (u32*)0x801c0034;
        u32* playerSheath13DLs = (u32*)0x801c005c;
        u32* playerSheath14DLs = (u32*)0x801c0084;
        u32* playerShieldDLs = (u32*)0x801c00ac;
        u32* playerSheathedSwordDLs = (u32*)0x801c00bc;
        u32* playerSwordSheathsDLs = (u32*)0x801c00d4;
        u32* playerLeftHandTwoHandSwordDLs = (u32*)0x801c00ec;
        u32* playerLeftHandOpenDLs = (u32*)0x801c0114;
        u32* playerLeftHandClosedDLs = (u32*)0x801c013c;
        u32* playerLeftHandOneHandSwordDLs = (u32*)0x801c0164;
        u32* playerEquipValueDLs = (u32*)0x801c018c;
        u32* playerRightHandOpenDLs = (u32*)0x801c01a4;
        u32* playerRightHandClosedDLs = (u32*)0x801c01cc;
        u32* playerRightHandBowDLs = (u32*)0x801c01f4;
        u32* playerRightHandInstrumentDLs = (u32*)0x801c021c;
        u32* playerRightHandHookshotDLs = (u32*)0x801c0244;
        u32* playerLeftHandBottleDLs = (u32*)0x801c026c;
        u32* playerFirstPersonLeftForearmDLs = (u32*)0x801c0294;
        u32* playerFirstPersonLeftHandDLs = (u32*)0x801c02a8;
        u32* playerFirstPersonRightShoulderDLs = (u32*)0x801c02bc;
        u32* playerFirstPersonRightHandDLs = (u32*)0x801c02d0;
        u32* playerFirstPersonRightHandHookshotDLs = (u32*)0x801c02e4;

        playerWaistDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_1;
        playerWaistDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_1;
        playerHandHoldingShieldDLs[0] = CUSTOM_OBJECT_MM_ADULT_LINK_2;
        playerHandHoldingShieldDLs[1] = CUSTOM_OBJECT_MM_ADULT_LINK_2;
        playerHandHoldingShieldDLs[2] = CUSTOM_OBJECT_MM_ADULT_LINK_3;
        playerHandHoldingShieldDLs[3] = CUSTOM_OBJECT_MM_ADULT_LINK_3;
        playerSheath12DLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_4;
        playerSheath12DLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_4;
        playerSheath13DLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_4;
        playerSheath13DLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_4;
        playerSheath14DLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_4;
        playerSheath14DLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_4;
        playerShieldDLs[0] = CUSTOM_OBJECT_MM_ADULT_LINK_5;
        playerShieldDLs[1] = CUSTOM_OBJECT_MM_ADULT_LINK_5;
        playerShieldDLs[2] = CUSTOM_OBJECT_MM_ADULT_LINK_6;
        playerShieldDLs[3] = CUSTOM_OBJECT_MM_ADULT_LINK_6;
        playerSheathedSwordDLs[0] = CUSTOM_OBJECT_MM_ADULT_LINK_7;
        playerSheathedSwordDLs[1] = CUSTOM_OBJECT_MM_ADULT_LINK_7;
        playerSheathedSwordDLs[2] = CUSTOM_OBJECT_MM_ADULT_LINK_8;
        playerSheathedSwordDLs[3] = CUSTOM_OBJECT_MM_ADULT_LINK_8;
        playerSheathedSwordDLs[4] = CUSTOM_OBJECT_MM_ADULT_LINK_9;
        playerSheathedSwordDLs[5] = CUSTOM_OBJECT_MM_ADULT_LINK_9;
        playerSwordSheathsDLs[0] = CUSTOM_OBJECT_MM_ADULT_LINK_10;
        playerSwordSheathsDLs[1] = CUSTOM_OBJECT_MM_ADULT_LINK_10;
        playerSwordSheathsDLs[2] = CUSTOM_OBJECT_MM_ADULT_LINK_11;
        playerSwordSheathsDLs[3] = CUSTOM_OBJECT_MM_ADULT_LINK_11;
        playerSwordSheathsDLs[4] = CUSTOM_OBJECT_MM_ADULT_LINK_12;
        playerSwordSheathsDLs[5] = CUSTOM_OBJECT_MM_ADULT_LINK_12;
        playerLeftHandTwoHandSwordDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_13;
        playerLeftHandTwoHandSwordDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_13;
        playerLeftHandOpenDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_14;
        playerLeftHandOpenDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_14;
        playerLeftHandClosedDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_15;
        playerLeftHandClosedDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_15;
        playerLeftHandOneHandSwordDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_16;
        playerLeftHandOneHandSwordDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_16;
        playerEquipValueDLs[0] = CUSTOM_OBJECT_MM_ADULT_LINK_17;
        playerEquipValueDLs[1] = CUSTOM_OBJECT_MM_ADULT_LINK_17;
        playerEquipValueDLs[2] = CUSTOM_OBJECT_MM_ADULT_LINK_18;
        playerEquipValueDLs[3] = CUSTOM_OBJECT_MM_ADULT_LINK_18;
        playerEquipValueDLs[4] = CUSTOM_OBJECT_MM_ADULT_LINK_19;
        playerEquipValueDLs[5] = CUSTOM_OBJECT_MM_ADULT_LINK_19;
        playerRightHandOpenDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_20;
        playerRightHandOpenDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_20;
        playerRightHandClosedDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_21;
        playerRightHandClosedDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_21;
        playerRightHandBowDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_22;
        playerRightHandBowDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_22;
        playerRightHandInstrumentDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_23;
        playerRightHandInstrumentDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_23;
        playerRightHandHookshotDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_24;
        playerRightHandHookshotDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_24;
        playerLeftHandBottleDLs[MM_PLAYER_FORM_HUMAN * 2] = CUSTOM_OBJECT_MM_ADULT_LINK_25;
        playerLeftHandBottleDLs[MM_PLAYER_FORM_HUMAN * 2 + 1] = CUSTOM_OBJECT_MM_ADULT_LINK_25;
        playerFirstPersonLeftForearmDLs[MM_PLAYER_FORM_HUMAN] = CUSTOM_OBJECT_MM_ADULT_LINK_4;
        playerFirstPersonLeftHandDLs[MM_PLAYER_FORM_HUMAN] = CUSTOM_OBJECT_MM_ADULT_LINK_15;
        playerFirstPersonRightShoulderDLs[MM_PLAYER_FORM_HUMAN] = CUSTOM_OBJECT_MM_ADULT_LINK_26;
        playerFirstPersonRightHandDLs[MM_PLAYER_FORM_HUMAN] = CUSTOM_OBJECT_MM_ADULT_LINK_27;
        playerFirstPersonRightHandHookshotDLs[MM_PLAYER_FORM_HUMAN] = CUSTOM_OBJECT_MM_ADULT_LINK_28;

        f32* meleeWeaponLengths = (f32*)0x801c0d78;
        meleeWeaponLengths[1] = 4000.0f;
        meleeWeaponLengths[2] = 4000.0f;
        meleeWeaponLengths[3] = 5500.0f;
        meleeWeaponLengths[4] = 5500.0f;

        u32* playerRightHandBowDLs2 = (u32*)0x801c0d94;
        *playerRightHandBowDLs2 = CUSTOM_OBJECT_MM_ADULT_LINK_29;

        u32* playerHeightJtbl = (u32*)0x801dca58;
        playerHeightJtbl[MM_PLAYER_FORM_HUMAN] = playerHeightJtbl[MM_PLAYER_FORM_ZORA];
    }

    if (!gCustomKeep)
    {
        comboLoadCustomKeep();
    }

    if (gSave.entranceIndex == ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_WEST && gSaveContext.nextCutscene == 0xfff7)
    {
        isEndOfGame = 1;
    }

    if ((gSave.entranceIndex == ENTR_MM_CLOCK_TOWN && gLastEntrance == ENTR_MM_CLOCK_TOWN_FROM_SONG_OF_TIME) || gSave.entranceIndex == ENTR_MM_CLOCK_TOWER_MOON_CRASH)
    {
        /* Song of Time / Moon crash */
        entrance = g.initialEntrance;
        applyCustomEntrance(&entrance);
        gSave.entranceIndex = entrance;
    }

    if (gSave.entranceIndex == ENTR_MM_WOODFALL_FROM_TEMPLE)
    {
        /* Woodfall from temple */
        if (!MM_GET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE))
        {
            gSave.entranceIndex = ENTR_MM_WARP_OWL_WOODFALL;
        }
    }

    /* Fix an issue with first day prompt */
    if ((gSave.day == 0 || (gSave.day == 1 && gSave.time == 0x4000)) && gSave.entranceIndex != ENTR_MM_CLOCK_TOWN)
    {
        gSave.day = 1;
        gSave.time = 0x4040; /* Little leeway to avoid being in the night -> day transition state */
        gSave.isNight = 0;
    }

    comboCacheClear();
    comboObjectsReset();

    /* Force alt beaver race */
    MM_SET_EVENT_WEEK(EV_MM_WEEK_BEAVER_RACE_ALT);

    /* Pre-feed the fish at the lab */
    MM_SET_EVENT_WEEK(MM_EV(81, 4));
    MM_SET_EVENT_WEEK(MM_EV(81, 5));
    MM_SET_EVENT_WEEK(MM_EV(81, 6));
    MM_SET_EVENT_WEEK(MM_EV(81, 7));
    MM_SET_EVENT_WEEK(MM_EV(82, 0));
    MM_SET_EVENT_WEEK(MM_EV(82, 1));

    /* Raise Woodfall Temple with setting enabled */
    if (comboConfig(CFG_MM_OPEN_WF))
        MM_SET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE);

    /* Make Biggoron move with setting enabled */
    if (comboConfig(CFG_MM_OPEN_SH))
        MM_SET_EVENT_WEEK(EV_MM_WEEK_SNOWHEAD_BLIZZARD);

    /* Make turtle surface with setting enabled */
    if (comboConfig(CFG_MM_OPEN_GB))
        MM_SET_EVENT_WEEK(EV_MM_WEEK_GREAT_BAY_TURTLE);

    if (gSave.entranceIndex == ENTR_MM_CLOCK_TOWER || gSave.entranceIndex == ENTR_MM_MOON)
        gNoTimeFlow = 1;

    Play_Init(play);

    gLastEntrance = gSave.entranceIndex;
    g.inGrotto = (play->sceneId == SCE_MM_GROTTOS);
    if (!g.inGrotto)
    {
        gLastScene = play->sceneId;
    }
    CustomTriggers_Spawn(play);
    comboSpawnItemGivers(play);
    comboSpawnCustomWarps(play);
    spawnSirloin(play);

    if (comboConfig(CFG_ER_ANY))
    {
        if (play->sceneId == SCE_MM_STONE_TOWER_INVERTED)
        {
            SetSwitchFlag(play, 0x14);
        }
        else if (play->sceneId == SCE_MM_STONE_TOWER)
        {
            ClearSwitchFlag(play, 0x14);
        }
    }

    if (gNoTimeFlow)
    {
        play->envCtx.sceneTimeSpeed = 0;
        gGameData->clockSpeed = 0;
    }

    if (isEndOfGame)
    {
        /* End game */
        gMmExtraFlags2.majora = 1;
        sendSelfMajorasMask();
        if (!comboGoalCond() && !g.isCreditWarp)
        {
            gSave.playerForm = MM_PLAYER_FORM_HUMAN;
            gSave.equippedMask = 0;
            gSave.day = 0;
            gSave.time = 0x3fff;
            comboSave(play, SF_NOCOMMIT);
            Sram_SaveNewDay(play);
            play->nextEntrance = ENTR_EXTENDED;
            g.nextEntrance = g.initialEntrance;
            play->transitionTrigger = TRANS_TRIGGER_NORMAL;
            play->transitionType = TRANS_TYPE_FADE_BLACK;
            return;
        }
        g.isCredits = 1;
    }

    if (gSave.entranceIndex == ENTR_MM_CLOCK_TOWER_FROM_CLOCK_TOWN)
    {
        comboGameSwitch(play, ENTR_OOT_MARKET_FROM_MASK_SHOP);
        return;
    }
}

void Play_UpdateWrapper(GameState_Play* play)
{
    /* Auto-press A during credits */
    if (g.isCredits)
        play->gs.input[0].pressed.buttons = (play->gs.frameCount & 1) ? A_BUTTON : 0;

    comboMenuTick();
    Debug_Input();
    malloc_check();
    comboCacheGarbageCollect();
    comboObjectsGC();
    checkTimeSkip(play);
    Play_Update(play);
    Debug_Update();
}

void Play_TransitionDone(GameState_Play* play)
{
    u32 entrance;
    s32 override;

    /* Resolve extended entrance */
    entrance = play->nextEntrance;
    switch (entrance)
    {
    case ENTR_EXTENDED:
        entrance = g.nextEntrance;
        break;
    case ENTR_FW_CROSS:
        entrance = gForeignSave.fw.entranceIndex | MASK_FOREIGN_ENTRANCE;
        gComboCtx.isFwSpawn = 1;
        break;
    }

    /* Handle grotto exits */
    if (entrance == ENTR_MM_INTERNAL_EXIT_GROTTO)
    {
        entrance = entrGrottoExit(play);
        if (entrance == ENTR_MM_INTERNAL_EXIT_GROTTO)
        {
            gIsEntranceOverride = 0;
            entrance = gSaveContext.respawn[3].entrance;
            gSaveContext.respawnFlag = 4;
        }
    }

    /* Handle transition override */
    if (gIsEntranceOverride)
    {
        gIsEntranceOverride = 0;
        override = comboEntranceOverride(entranceForOverride(entrance));
        if (override != -1)
            entrance = (u32)override;
    }

    applyCustomEntrance(&entrance);

    /* Check for foreign */
    if (entrance & MASK_FOREIGN_ENTRANCE)
    {
        comboGameSwitch(play, entrance & ~MASK_FOREIGN_ENTRANCE);
    }
    else
    {
        play->nextEntrance = entrance & 0xffff;
    }
}

void Play_SetupRespawnPointRaw(GameState_Play* play, int respawnId, int playerParams)
{
    Actor_Player* link;

    link = GET_LINK(play);
    Play_SetRespawnData(play, respawnId, gSave.entranceIndex, gPlay->roomCtx.curRoom.id, playerParams, &link->base.world.pos, link->base.rot2.y);
}

void Play_SetupRespawnPoint(GameState_Play* play, int respawnId, int playerParams)
{
    if (!comboConfig(CFG_ER_GROTTOS) && play->sceneId == SCE_MM_GROTTOS)
        return;

    Play_SetupRespawnPointRaw(play, respawnId, playerParams);
}

PATCH_FUNC(0x80169e6c, Play_SetupRespawnPoint);

void CutsceneTransitionHook(GameState_Play* play)
{
    /* Default hook */
    void (*DefaultHook)(GameState_Play*);
    DefaultHook = (void*)0x801306a4;
    DefaultHook(play);

    if (gSaveContext.gameMode)
        return;

    if (gPlay->sceneId == SCE_MM_MUSIC_BOX_HOUSE && gPlay->nextEntrance == ENTR_MM_IKANA_CANYON_FROM_MUSIC_BOX_HOUSE)
    {
        /* Cutscene when getting kicked by Pam */
        gIsEntranceOverride = 1;
        return;
    }
}
