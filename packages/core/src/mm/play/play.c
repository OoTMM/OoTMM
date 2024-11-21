#include <combo.h>
#include <combo/net.h>
#include <combo/menu.h>
#include <combo/entrance.h>
#include <combo/time.h>
#include <combo/custom.h>
#include <combo/debug.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/item.h>
#include <combo/global.h>
#include <combo/multi.h>
#include <combo/context.h>
#include <combo/audio.h>
#include <actors/Obj_Grass/Obj_Grass.h>

/* Grass hooks */
ObjGrass* gObjGrass;
static u8 sNeedsScreenClear;

void ObjGrass_GetID(int* dstPackId, int* dstBushId, ObjGrassElement* grass)
{
    u32 offset;
    u32 offsetPack;
    u32 offsetBush;
    int packId;
    int bushId;

    /* Extract the pack ID */
    offset = (u32)grass - (u32)gObjGrass;
    offsetPack = (offset - offsetof(ObjGrassGroup, elements) - offsetof(ObjGrass, grassGroups));
    packId = offsetPack / sizeof(ObjGrassGroup);

    /* Extract the bush ID */
    offset = (u32)grass - ((u32)&gObjGrass->grassGroups[packId]);
    offsetBush = offset - offsetof(ObjGrassGroup, elements);
    bushId = offsetBush / sizeof(ObjGrassElement);

    /* Store the IDs */
    *dstPackId = packId;
    *dstBushId = bushId;
}

void ObjGrass_GetXflag(Xflag* xflag, ObjGrassElement* grassElem)
{
    int packId;
    int bushId;

    ObjGrass_GetID(&packId, &bushId, grassElem);
    memcpy(xflag, &gObjGrass->xflag[packId], sizeof(Xflag));
    xflag->sliceId = bushId;
}

PlayState* gPlay;
int gNoTimeFlow;

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

    if (!Config_Flag(CFG_MULTIPLAYER))
        return;

    gi = GI_MM_MASK_MAJORA;
    npc = NPC_MM_MAJORA;

    net = netMutexLock();
    netWaitCmdClear();
    bzero(&net->cmdOut, sizeof(net->cmdOut));
    net->cmdOut.op = NET_OP_ITEM_SEND;
    net->cmdOut.itemSend.playerFrom = gComboConfig.playerId;
    net->cmdOut.itemSend.playerTo = gComboConfig.playerId;
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

    entrance = gSave.entrance;
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
        case 0x19: /* Path to Mountain Village */
            gSaveContext.nextCutscene = 0xfff0;
            break;
        case 0x58: /* Path to Snowhead */
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

    gSave.entrance = (entranceKey << 9) | (entrance & 0x1ff);
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

    /* Copy to the void and death respawn */
    memcpy(&gSaveContext.respawn[0], rs, sizeof(RespawnData));
    memcpy(&gSaveContext.respawn[2], rs, sizeof(RespawnData));

    /* Set the respawn flags */
    gSaveContext.respawnFlag = 4;
    *entrance = rs->entrance;
}

static u32 entrGrottoExit(PlayState* play)
{
    if (!Config_Flag(CFG_ER_GROTTOS))
        return ENTR_MM_INTERNAL_EXIT_GROTTO;

    switch (play->sceneId)
    {
    case SCE_MM_GROTTOS:
        switch (play->roomCtx.curRoom.num)
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

static void spawnSirloin(PlayState* play)
{
    if (!gSharedCustomSave.storedSirloin)
        return;
    if (play->sceneId != SCE_MM_MOUNTAIN_VILLAGE_WINTER)
        return;
    if (MM_CHECK_EVENT_INF(EV_MM_WEEK_DUNGEON_SH))
        return;

    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_MM, -1025.f, 8.f, 400.f, 0.f, 0.f, 0.f, 0x8000);
}

static void Play_FixupSpawnTime(void)
{
    int firstHalfDay;

    /* Check if we need to skip forward in time */
    if (!(gSave.day == 0 || (gSave.day == 1 && gSave.time == 0x4000)))
        return;

    /* Set/clear 1st day events */
    Sram_ClearFlagsAtDawnOfTheFirstDay();

    firstHalfDay = 6;
    for (int i = 0; i < 6; ++i)
    {
        if (gSharedCustomSave.mm.halfDays & (1 << i))
        {
            firstHalfDay = i;
            break;
        }
    }

    if (firstHalfDay == 0)
    {
        /* Work around a vanilla bug */
        if (gSave.entrance != ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER)
        {
            gSave.day = 1;
            gSave.time = CLOCK_TIME(6, 1);
        }
        return;
    }

    /* We start on a later clock */
    if (firstHalfDay & 1)
    {
        gSave.day = (firstHalfDay / 2) + 1;
        gSave.time = 0xc000;
    }
    else
    {
        gSave.day = (firstHalfDay / 2);
        gSave.time = 0x3fff;
    }
}

void ComboPlay_JpLayout(PlayState* play);

void Play_RoomChangeHook(PlayState* play)
{
    ComboPlay_JpLayout(play);
}

void Play_CheckRoomChangeHook(PlayState* play)
{
    if (g.prevRoom != play->roomCtx.curRoom.num)
    {
        g.prevRoom = play->roomCtx.curRoom.num;
        Play_RoomChangeHook(play);
    }
}

void hookPlay_Init(PlayState* play)
{
    u32 entrance;

    gPlay = play;
    int isEndOfGame;

    /* Init */
    g.prevRoom = -1;
    gIsEntranceOverride = 0;
    g.decoysCount = 0;
    isEndOfGame = 0;
    gActorCustomTriggers = NULL;
    gMultiMarkChests = 0;
    gMultiMarkCollectibles = 0;
    gMultiMarkSwitch0 = 0;
    gMultiMarkSwitch1 = 0;
    Multi_ResetWisps();

    if (Config_Flag(CFG_ER_OVERWORLD) || Config_Flag(CFG_ER_INDOORS))
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

    if (!g.customKeep)
    {
        comboLoadCustomKeep();
    }

    if (gSave.entrance == ENTR_MM_TERMINA_FIELD_FROM_CLOCK_TOWN_WEST && gSaveContext.nextCutscene == 0xfff7)
    {
        isEndOfGame = 1;
    }

    if ((gSave.entrance == ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER && gLastEntrance == ENTR_MM_CLOCK_TOWN_FROM_SONG_OF_TIME) || gSave.entrance == ENTR_MM_CLOCK_TOWER_MOON_CRASH)
    {
        /* Song of Time / Moon crash */
        gNoTimeFlow = 0;
        entrance = g.initialEntrance;
        applyCustomEntrance(&entrance);
        gSave.entrance = entrance;
    }

    if (gSave.entrance == ENTR_MM_WOODFALL_FROM_TEMPLE)
    {
        /* Woodfall from temple */
        if (!MM_GET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE))
        {
            gSave.entrance = ENTR_MM_WARP_OWL_WOODFALL;
        }
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

    /* Tingle already knows about the player */
    MM_SET_EVENT_WEEK(EV_MM_WEEK_TINGLE_TALKED);

    /* Raise Woodfall Temple with setting enabled */
    if (Config_Flag(CFG_MM_OPEN_WF))
        MM_SET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE);

    /* Make Biggoron move with setting enabled */
    if (Config_Flag(CFG_MM_OPEN_SH))
        MM_SET_EVENT_WEEK(EV_MM_WEEK_SNOWHEAD_BLIZZARD);

    /* Make turtle surface with setting enabled */
    if (Config_Flag(CFG_MM_OPEN_GB))
        MM_SET_EVENT_WEEK(EV_MM_WEEK_GREAT_BAY_TURTLE);

    Play_FixupSpawnTime();
    Play_Init(play);

    gLastEntrance = gSave.entrance;
    g.inGrotto = (play->sceneId == SCE_MM_GROTTOS);
    if (!g.inGrotto)
    {
        gLastScene = play->sceneId;
    }
    CustomTriggers_Spawn(play);
    comboSpawnCustomWarps(play);
    spawnSirloin(play);
    ComboPlay_SpawnExtraSigns(play);
    Play_CheckRoomChangeHook(play);

    if (Config_Flag(CFG_ER_ANY))
    {
        if (play->sceneId == SCE_MM_STONE_TOWER_INVERTED)
        {
            Flags_SetSwitch(play, 0x14);
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
        if (!Config_IsGoal() && !g.isCreditWarp)
        {
            gSave.playerForm = MM_PLAYER_FORM_HUMAN;
            gSave.equippedMask = 0;
            gSave.day = 0;
            gSave.time = 0x3fff;
            Save_DoSave(play, SF_NOCOMMIT);
            Sram_SaveNewDay(play);
            play->nextEntrance = ENTR_EXTENDED;
            g.nextEntrance = g.initialEntrance;
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_FADE_BLACK;
            return;
        }
        g.isCredits = 1;
    }
}

void Play_DrawMainWrapper(PlayState* play)
{
    if (sNeedsScreenClear)
    {
        func_8012CF0C(play->state.gfxCtx, true, true, 0, 0, 0);
        sNeedsScreenClear--;
        return;
    }

    Play_DrawMain(play);
}

PATCH_CALL(0x80168f48, Play_DrawMainWrapper);

void Play_MainWrapper(PlayState* play)
{
    Player* link;
    /* Auto-press A during credits */
    if (g.isCredits)
        play->state.input[0].press.button = (play->state.frameCount & 1) ? A_BUTTON : 0;

    comboMenuTick();
    Debug_Input();
    malloc_check();
    comboCacheGarbageCollect();
    comboObjectsGC();
    link = GET_PLAYER(play);
    Player_TryUpdateForm(link, play);
    Multi_Update(play);
    Play_Main(play);
    Play_CheckRoomChangeHook(play);
    Audio_DisplayMusicName(play);
    Debug_Update();
}

void Play_TransitionDone(PlayState* play)
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
        entrance = gForeignSave.info.fw.entrance | MASK_FOREIGN_ENTRANCE;
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
        g.isNextEntranceInitialSong = (entrance == ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER);
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

void Play_SetupRespawnPointRaw(PlayState* play, int respawnId, int playerParams)
{
    Player* link;

    link = GET_PLAYER(play);
    Play_SetRespawnData(play, respawnId, gSave.entrance, gPlay->roomCtx.curRoom.num, playerParams, &link->actor.world.pos, link->actor.shape.rot.y);
}

void Play_SetupRespawnPoint(PlayState* play, int respawnId, int playerParams)
{
    if (!Config_Flag(CFG_ER_GROTTOS) && play->sceneId == SCE_MM_GROTTOS)
        return;

    Play_SetupRespawnPointRaw(play, respawnId, playerParams);
}

PATCH_FUNC(0x80169e6c, Play_SetupRespawnPoint);

void CutsceneTransitionHook(PlayState* play)
{
    /* Default hook */
    void (*DefaultHook)(PlayState*);
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

void Play_FastInit(GameState* gs)
{
    u32 entrance;

    /* Init the play state */
    gs->nextGameStateInit = Play_Init;
    gs->nextGameStateSize = sizeof(PlayState);

    /* Load the save file */
    gSaveContext.fileNum = gComboCtx.saveIndex;
    Sram_OpenSave(NULL, NULL);

    /* Set stuff */
    gSaveContext.gameMode = GAMEMODE_NORMAL;
    gSaveContext.showTitleCard = TRUE;

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
    if (Config_Flag(CFG_ER_ANY))
        g.initialEntrance = entrance;
    else
        g.initialEntrance = ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER;
    applyCustomEntrance(&entrance);
    gSave.entrance = entrance;
    g.isNextEntranceInitialSong = (entrance == ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER);

    /* Fixup the scene/setup */
    fixupOriginalSceneSetup();

    /* Handle shuffled entrance */
    switch (gSave.entrance)
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
    sNeedsScreenClear = 2;
}
