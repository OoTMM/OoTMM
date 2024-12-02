#include <combo.h>
#include <combo/souls.h>
#include <combo/net.h>
#include <combo/menu.h>
#include <combo/entrance.h>
#include <combo/debug.h>
#include <combo/magic.h>
#include <combo/config.h>
#include <combo/global.h>
#include <combo/dpad.h>
#include <combo/multi.h>
#include <combo/context.h>
#include <combo/dungeon.h>
#include <combo/audio.h>

PlayState* gPlay;

void Play_GoToFileSelect(GameState* gs)
{
    gs->nextGameStateInit = FileSelect_Init;
    gs->nextGameStateSize = sizeof(FileSelectState);
}

static int isRainbowBridgeOpen(void)
{
    if (Config_Flag(CFG_OOT_BRIDGE_CUSTOM) && !Config_SpecialCond(SPECIAL_BRIDGE))
        return 0;

    if (Config_Flag(CFG_OOT_BRIDGE_VANILLA) && !(
        gOotSave.info.inventory.quest.medallionShadow
        && gOotSave.info.inventory.quest.medallionSpirit
        && gOotSave.info.inventory.items[ITS_OOT_ARROW_LIGHT] == ITEM_OOT_ARROW_LIGHT
    ))
        return 0;

    if (Config_Flag(CFG_OOT_BRIDGE_MEDALLIONS) && !(
        gOotSave.info.inventory.quest.medallionLight
        && gOotSave.info.inventory.quest.medallionForest
        && gOotSave.info.inventory.quest.medallionFire
        && gOotSave.info.inventory.quest.medallionWater
        && gOotSave.info.inventory.quest.medallionShadow
        && gOotSave.info.inventory.quest.medallionSpirit
    ))
        return 0;

    return 1;
}

static void eventFixes(PlayState* play)
{
    /* Skip forest temple cutscene */
    if (gSave.entrance == ENTR_OOT_TEMPLE_FOREST)
    {
        u32 tmp;
        tmp = gSave.info.perm[SCE_OOT_TEMPLE_FOREST].switches;
        if ((tmp & 0xf0000000) != 0xf0000000)
            tmp |= 0x08000000;
        gSave.info.perm[SCE_OOT_TEMPLE_FOREST].switches = tmp;
    }

    /* Ruto fixes */
    if (gSave.entrance == ENTR_OOT_JABU_JABU)
    {
        /* Skip the intro cutscene */
        u16 tmp;
        tmp = gSave.info.eventsMisc[20];
        if (!(tmp & 0xff))
            tmp |= 0x1e;
        gSave.info.eventsMisc[20] = tmp;

        /* Places Ruto on first floor */
        if (!(Config_IsMq(MQ_JABU_JABU)))
            BITMAP16_SET(gSave.info.eventsMisc, EV_OOT_INF_RUTO_TOP_FLOOR);

        /* Ruto kidnap fixes */
        if (BITMAP16_GET(gSave.info.eventsMisc, EV_OOT_INF_RUTO_KIDNAPPED) || BITMAP16_GET(gSave.info.eventsMisc, EV_OOT_INF_RUTO_GOT_SAPPHIRE))
        {
            /* Un-kidnap ruto */
            BITMAP16_CLEAR(gSave.info.eventsMisc, EV_OOT_INF_RUTO_KIDNAPPED);
            BITMAP16_CLEAR(gSave.info.eventsMisc, EV_OOT_INF_RUTO_GOT_SAPPHIRE);
        }
    }

    /* Set the rainbow bridge flag */
    if (isRainbowBridgeOpen())
    {
        SetEventChk(EV_OOT_CHK_RAINBOW_BRIDGE);
    }

    if(Config_Flag(CFG_OOT_PLANTED_BEANS))
    {
        gOotSave.info.perm[SCE_OOT_GRAVEYARD].switches |= 0x00000008;
        gOotSave.info.perm[SCE_OOT_ZORA_RIVER].switches |= 0x00000008;
        gOotSave.info.perm[SCE_OOT_KOKIRI_FOREST].switches |= 0x00000200;
        gOotSave.info.perm[SCE_OOT_LAKE_HYLIA].switches |= 0x00000002;
        gOotSave.info.perm[SCE_OOT_GERUDO_VALLEY].switches |= 0x00000008;
        gOotSave.info.perm[SCE_OOT_LOST_WOODS].switches |= 0x00040010;
        gOotSave.info.perm[SCE_OOT_DESERT_COLOSSUS].switches |= 0x01000000;
        gOotSave.info.perm[SCE_OOT_DEATH_MOUNTAIN_TRAIL].switches |= 0x00000040;
        gOotSave.info.perm[SCE_OOT_DEATH_MOUNTAIN_CRATER].switches |= 0x00000008;
        gOotSave.info.inventory.items[ITS_OOT_MAGIC_BEAN] = ITEM_OOT_MAGIC_BEAN;
    }

    if(Config_Flag(CFG_OOT_OPEN_JABU_JABU))
        SetEventChk(EV_OOT_CHK_JABU_JABU_OPEN);

    if(Config_Flag(CFG_OOT_OPEN_SHADOW_TEMPLE))
        gSave.info.perm[SCE_OOT_GRAVEYARD].switches |= 0xc0000000;

    if(Config_Flag(CFG_OOT_OPEN_DODONGO_CAVERN))
        gSave.info.perm[SCE_OOT_DEATH_MOUNTAIN_TRAIL].switches |= 0x00000010;

    if(Config_Flag(CFG_OOT_OPEN_WATER_TEMPLE))
        gSave.info.perm[SCE_OOT_LAKE_HYLIA].switches |= 0x80000000;

    if(Config_Flag(CFG_OOT_OPEN_WELL))
        SetEventChk(EV_OOT_CHK_WELL_DRAINED);

}

static void sendSelfTriforce(void)
{
    NetContext* net;
    int npc;
    s16 gi;

    if (!Config_Flag(CFG_MULTIPLAYER))
        return;

    gi = GI_OOT_TRIFORCE_FULL;
    npc = NPC_OOT_GANON;

    net = netMutexLock();
    netWaitCmdClear();
    bzero(&net->cmdOut, sizeof(net->cmdOut));
    net->cmdOut.op = NET_OP_ITEM_SEND;
    net->cmdOut.itemSend.playerFrom = gComboConfig.playerId;
    net->cmdOut.itemSend.playerTo = gComboConfig.playerId;
    net->cmdOut.itemSend.game = 0;
    net->cmdOut.itemSend.gi = gi;
    net->cmdOut.itemSend.key = ((u32)OV_NPC << 24) | npc;
    net->cmdOut.itemSend.flags = 0;
    netMutexUnlock();

    /* Mark the NPC as obtained */
    BITMAP8_SET(gSharedCustomSave.oot.npc, npc);
}

static void endGame(void)
{
    u8  tmpAge;
    u16 tmpNextCutscene;
    s32 tmpCutscene;
    u16 tmpEntrance;
    u16 tmpSceneId;

    /* Ignore credit warp */
    if (g.isCreditWarp)
        return;

    /* Flag ganon as beaten */
    gOotExtraFlags.ganon = 1;

    /* Send self triforce */
    sendSelfTriforce();

    /* Save tmp gameplay values (in case majora was beaten too) */
    tmpAge = gSave.age;
    tmpNextCutscene = gSaveContext.nextCutscene;
    tmpCutscene = gSave.cutscene;
    tmpEntrance = gSave.entrance;
    tmpSceneId = gSave.info.sceneId;

    /* Edit gameplay values for end of game save */
    gSave.age = AGE_ADULT;
    gSaveContext.nextCutscene = 0;
    gSave.cutscene = 0;
    if (Config_Flag(CFG_ER_ANY))
    {
        gSave.entrance = ENTR_OOT_GANON_TOWER;
        gSave.info.sceneId = SCE_OOT_GANON_TOWER;
    }
    else
    {
        gSave.entrance = ENTR_OOT_GANON_CASTLE_EXTERIOR_FROM_CASTLE;
        gSave.info.sceneId = SCE_OOT_GANON_CASTLE_EXTERIOR;
    }

    /* Save */
    Save_DoSave(NULL, SF_PASSIVE);

    /* Restore gameplay values to play the cutscene if majora was beaten too */
    if (Config_IsGoal())
    {
        gSave.age = tmpAge;
        gSaveContext.nextCutscene = tmpNextCutscene;
        gSave.cutscene = tmpCutscene;
        gSave.entrance = tmpEntrance;
        gSave.info.sceneId = tmpSceneId;
        g.isCredits = 1;
    }
}

static u32 entranceForOverride(u32 entrance)
{
    switch (entrance)
    {
    case ENTR_OOT_ZORA_RIVER_FROM_FIELD_WATER:
        /* Water entrance to zora river */
        return ENTR_OOT_ZORA_RIVER_FROM_FIELD;
    case ENTR_OOT_FIELD_FROM_ZORA_RIVER_WATER:
        /* Water entrance to hyrule */
        return ENTR_OOT_FIELD_FROM_ZORA_RIVER;
    case ENTR_OOT_HYRULE_CASTLE_FROM_FAIRY:
        return gLastScene == SCE_OOT_GREAT_FAIRY_FOUNTAIN_UPGRADES ? ENTR_OOT_OUTSIDE_GANON_FROM_FAIRY : ENTR_OOT_HYRULE_CASTLE_FROM_FAIRY;
    case ENTR_OOT_LOST_WOODS_FROM_KOKIRI_FOREST:
        return gLastScene == SCE_OOT_LOST_WOODS ? ENTR_OOT_LOST_WOODS_FROM_LOST_WOODS_NORTH : ENTR_OOT_LOST_WOODS_FROM_KOKIRI_FOREST;
    case ENTR_OOT_GROTTO_TYPE_GENERIC:
        switch (gGrottoData & 0x1f)
        {
        case 0x0c: return ENTR_OOT_GROTTO_GENERIC_KOKIRI_FOREST;
        case 0x14: return ENTR_OOT_GROTTO_GENERIC_LOST_WOODS;
        case 0x08: return ENTR_OOT_GROTTO_GENERIC_KAKARIKO;
        case 0x17: return ENTR_OOT_GROTTO_GENERIC_DMT;
        case 0x1a: return ENTR_OOT_GROTTO_GENERIC_DMC;
        case 0x09: return ENTR_OOT_GROTTO_GENERIC_RIVER;
        case 0x02: return ENTR_OOT_GROTTO_GENERIC_HF_SOUTHEAST;
        case 0x03: return ENTR_OOT_GROTTO_GENERIC_HF_OPEN;
        case 0x00: return ENTR_OOT_GROTTO_GENERIC_HF_MARKET;
        }
        UNREACHABLE();
    case ENTR_OOT_GROTTO_TYPE_FAIRY:
        switch (gLastScene)
        {
        case SCE_OOT_SACRED_FOREST_MEADOW: return ENTR_OOT_GROTTO_FAIRY_SFM;
        case SCE_OOT_HYRULE_FIELD: return ENTR_OOT_GROTTO_FAIRY_HF;
        case SCE_OOT_ZORA_RIVER: return ENTR_OOT_GROTTO_FAIRY_RIVER;
        case SCE_OOT_ZORA_DOMAIN: return ENTR_OOT_GROTTO_FAIRY_DOMAIN;
        case SCE_OOT_GERUDO_FORTRESS: return ENTR_OOT_GROTTO_FAIRY_FORTRESS;
        }
        UNREACHABLE();
    case ENTR_OOT_GROTTO_TYPE_SCRUB2:
        switch (gLastScene)
        {
        case SCE_OOT_SACRED_FOREST_MEADOW: return ENTR_OOT_GROTTO_SCRUBS2_SFM;
        case SCE_OOT_ZORA_RIVER: return ENTR_OOT_GROTTO_SCRUBS2_RIVER;
        case SCE_OOT_GERUDO_VALLEY: return ENTR_OOT_GROTTO_SCRUBS2_VALLEY;
        case SCE_OOT_DESERT_COLOSSUS: return ENTR_OOT_GROTTO_SCRUBS2_COLOSSUS;
        }
        UNREACHABLE();
    case ENTR_OOT_GROTTO_TYPE_SCRUB3:
        switch (gLastScene)
        {
        case SCE_OOT_LON_LON_RANCH: return ENTR_OOT_GROTTO_SCRUBS3_RANCH;
        case SCE_OOT_GORON_CITY: return ENTR_OOT_GROTTO_SCRUBS3_GORON_CITY;
        case SCE_OOT_DEATH_MOUNTAIN_CRATER: return ENTR_OOT_GROTTO_SCRUBS3_DMC;
        case SCE_OOT_LAKE_HYLIA: return ENTR_OOT_GROTTO_SCRUBS3_LAKE;
        }
        UNREACHABLE();
    default:
        return entrance;
    }
}


static const u8 kGrottoDataGeneric[] = { 0x0c, 0x14, 0x08, 0x17, 0x1a, 0x09, 0x02, 0x03, 0x00 };

static const u8 kGrottoDataFairy[] = {
    SCE_OOT_SACRED_FOREST_MEADOW,
    SCE_OOT_HYRULE_FIELD,
    SCE_OOT_ZORA_RIVER,
    SCE_OOT_ZORA_DOMAIN,
    SCE_OOT_GERUDO_FORTRESS,
};

static const u8 kGrottoDataScrubs2[] = {
    SCE_OOT_SACRED_FOREST_MEADOW,
    SCE_OOT_ZORA_RIVER,
    SCE_OOT_GERUDO_VALLEY,
    SCE_OOT_DESERT_COLOSSUS,
};

static const u8 kGrottoDataScrubs3[] = {
    SCE_OOT_LON_LON_RANCH,
    SCE_OOT_GORON_CITY,
    SCE_OOT_DEATH_MOUNTAIN_CRATER,
    SCE_OOT_LAKE_HYLIA,
};

typedef struct
{
    u16 entrance;
    u8  room;
    s16 pos[3];
}
GrottoExit;

static const GrottoExit kGrottoExits[] = {
    /* Generic Grottos */
    { ENTR_OOT_KOKIRI_FOREST_FROM_LOST_WOODS,    0, {  -512,  380, -1224 } },
    { ENTR_OOT_LOST_WOODS_FROM_KOKIRI_FOREST,    2, {   915,    0,  -925 } },
    { ENTR_OOT_KAKARIKO_FROM_GRANNY,             0, {   860,   80,  -260 } },
    { ENTR_OOT_DEATH_MOUNTAIN_FROM_GORON_CITY,   0, {  -383, 1386, -1206 } },
    { ENTR_OOT_DEATH_MOUNTAIN_CRATER,            1, {    40, 1233,  1770 } },
    { ENTR_OOT_ZORA_RIVER_FROM_FIELD,            0, {   360,  570,   130 } },
    { ENTR_OOT_FIELD_FROM_LAKE_HYLIA,            0, {  -270, -500, 12350 } },
    { ENTR_OOT_FIELD_FROM_LAKE_HYLIA,            0, { -4030, -700, 13860 } },
    { ENTR_OOT_FIELD_FROM_MARKET_ENTRANCE,       0, { -1425,    0,   810 } },

    /* Fairy Fountains */
    { ENTR_OOT_SACRED_FOREST_MEADOW,        0,  {    45,    0,   220 } },
    { ENTR_OOT_FIELD_FROM_MARKET_ENTRANCE,  0,  { -4450, -300,  -425 } },
    { ENTR_OOT_ZORA_RIVER_FROM_FIELD,       0,  {   670,  570,  -365 } },
    { ENTR_OOT_ZORA_DOMAIN,                 1,  {  -860,   14,  -470 } },
    { ENTR_OOT_GERUDO_FORTRESS_FROM_VALLEY, 0,  {   376,  333, -1564 } },

    /* Double Scrubs */
    { ENTR_OOT_WARP_SONG_MEADOW,           0, {  310,  480, -2300 } },
    { ENTR_OOT_ZORA_RIVER_FROM_FIELD,      0, { -1630, 100,  -130 } },
    { ENTR_OOT_GERUDO_VALLEY_FROM_TENT,    0, { -1323,  15,  -969 } },
    { ENTR_OOT_DESERT_COLOSSUS_FROM_FAIRY, 0, {    60, -32, -1300 } },

    /* Triple scrubs */
    { ENTR_OOT_LON_LON_RANCH_FROM_HOUSE,                0, {  1800,    0,  1500 } },
    { ENTR_OOT_GORON_CITY,                              3, {  1100,  580, -1190 } },
    { ENTR_OOT_CRATER_FROM_GORON_CITY,                  1, { -1699,  722,  -472 } },
    { ENTR_OOT_WARP_SONG_LAKE,                          0, { -3040, -1033, 6075 } },

    /* Other Grottos */
    { ENTR_OOT_LOST_WOODS_FROM_KOKIRI_FOREST,   8, {   670,    0, -2520 } }, /* OOT_GROTTO_EXIT_SCRUB_UPGRADE */
    { ENTR_OOT_FIELD_FROM_LAKE_HYLIA,           0, { -4990, -700, 13820 } }, /* OOT_GROTTO_EXIT_SCRUB_HEART_PIECE */
    { ENTR_OOT_LOST_WOODS_FROM_MEADOW,          6, {    80,  -20, -1600 } }, /* OOT_GROTTO_EXIT_DEKU_THEATER */
    { ENTR_OOT_SACRED_FOREST_MEADOW,            0, {  -195,    0,  1900 } }, /* OOT_GROTTO_EXIT_WOLFOS */
    { ENTR_OOT_FIELD_FROM_MARKET_ENTRANCE,      0, { -4945, -300,  2835 } }, /* OOT_GROTTO_EXIT_TEKTITE */
    { ENTR_OOT_FIELD_FROM_GERUDO_VALLEY,        0, { -7870, -300,  6920 } }, /* OOT_GROTTO_EXIT_FIELD_COW */
    { ENTR_OOT_FIELD_FROM_KAKARIKO,             0, {  2060,   20,  -170 } }, /* OOT_GROTTO_EXIT_FIELD_TREE */
    { ENTR_OOT_DEATH_MOUNTAIN_FROM_KAKARIKO,    0, {  -688, 1946,  -285 } }, /* OOT_GROTTO_EXIT_TRAIL_COW */
    { ENTR_OOT_HYRULE_CASTLE,                   0, {   996, 1571,   844 } }, /* OOT_GROTTO_EXIT_CASTLE */
    { ENTR_OOT_KAKARIKO_FROM_FIELD,             0, {  -400,    0,   400 } }, /* OOT_GROTTO_EXIT_REDEAD */
    { ENTR_OOT_GERUDO_VALLEY_FROM_FIELD,        0, {   280, -555,  1470 } }, /* OOT_GROTTO_EXIT_OCTOROK */
};

static const OotRespawnData kSpiritTempleMainEntrance = {
    { -5.0f, -150.f, 754.0f },
    DEG_TO_BINANG(180),
    0xdff,
    ENTR_OOT_TEMPLE_SPIRIT,
    0,
    0,
    0,
    0,
};

static void applyGrottoExit(u32* entrance, int id)
{
    OotRespawnData* rs;
    const GrottoExit* ge;

    /* Set the grotto exit */
    rs = &gSaveContext.respawn[1];
    ge = &kGrottoExits[id];
    rs->pos.x = ge->pos[0];
    rs->pos.y = ge->pos[1];
    rs->pos.z = ge->pos[2];
    rs->yaw = 0;
    rs->entrance = ge->entrance;
    rs->playerParams = 0x04ff;
    rs->data = 0;
    rs->roomIndex = ge->room;
    rs->tempSwitchFlags = 0;
    rs->tempCollectFlags = 0;

    /* Set the void respawn */
    memcpy(&gSaveContext.respawn[0], rs, sizeof(OotRespawnData));

    /* Set the respawn flags */
    gSaveContext.respawnFlag = 2;
    gSaveContext.nextTransitionType = 3;
    *entrance = rs->entrance;
}

static void applyCustomEntrance(u32* entrance)
{
    u32 id;

    id = *entrance;
    if (id >= ENTR_OOT_GROTTO_GENERIC_KOKIRI_FOREST && id <= ENTR_OOT_GROTTO_GENERIC_HF_MARKET)
    {
        id -= ENTR_OOT_GROTTO_GENERIC_KOKIRI_FOREST;
        *entrance = ENTR_OOT_GROTTO_TYPE_GENERIC;
        gGrottoData &= ~0x1f;
        gGrottoData |= kGrottoDataGeneric[id];
    }
    else if (id >= ENTR_OOT_GROTTO_FAIRY_SFM && id <= ENTR_OOT_GROTTO_FAIRY_FORTRESS)
    {
        id -= ENTR_OOT_GROTTO_FAIRY_SFM;
        *entrance = ENTR_OOT_GROTTO_TYPE_FAIRY;
        gLastScene = kGrottoDataFairy[id];
    }
    else if (id >= ENTR_OOT_GROTTO_SCRUBS2_SFM && id <= ENTR_OOT_GROTTO_SCRUBS2_COLOSSUS)
    {
        id -= ENTR_OOT_GROTTO_SCRUBS2_SFM;
        *entrance = ENTR_OOT_GROTTO_TYPE_SCRUB2;
        gLastScene = kGrottoDataScrubs2[id];
    }
    else if (id >= ENTR_OOT_GROTTO_SCRUBS3_RANCH && id <= ENTR_OOT_GROTTO_SCRUBS3_LAKE)
    {
        id -= ENTR_OOT_GROTTO_SCRUBS3_RANCH;
        *entrance = ENTR_OOT_GROTTO_TYPE_SCRUB3;
        gLastScene = kGrottoDataScrubs3[id];
    }
    else if (id >= ENTR_OOT_GROTTO_EXIT_GENERIC_KOKIRI_FOREST && id <= ENTR_OOT_GROTTO_EXIT_OCTOROK)
    {
        id -= ENTR_OOT_GROTTO_EXIT_GENERIC_KOKIRI_FOREST;
        applyGrottoExit(entrance, id);
    }
}

static void playAdjustEntrance(PlayState* play)
{
    /* Handle custom entrance IDs */
    switch (gSave.entrance)
    {
    case ENTR_OOT_SPAWN_ADULT:
        gSave.entrance = ENTR_OOT_WARP_SONG_TEMPLE;
        break;
    case ENTR_OOT_OUTSIDE_GANON_FROM_FAIRY:
        gSave.entrance = ENTR_OOT_HYRULE_CASTLE_FROM_FAIRY;
        break;
    case ENTR_OOT_LOST_WOODS_FROM_LOST_WOODS_NORTH:
        gSave.entrance = ENTR_OOT_LOST_WOODS_FROM_KOKIRI_FOREST;
        break;
    case ENTR_OOT_CASTLE_STEALTH:
        gSave.entrance = ENTR_OOT_CASTLE_COURTYARD;
        break;
    case ENTR_OOT_CASTLE_STEALTH_FROM_COURTYARD:
        gSave.entrance = ENTR_OOT_GANON_CASTLE_EXTERIOR_FROM_CASTLE; /* Stealth exit as child */
        break;
    case ENTR_OOT_SAGES_CHAMBER_END:
        endGame();
        break;
    case ENTR_OOT_BOSS_GANON2:
        if (!comboHasSoulOot(GI_OOT_SOUL_NPC_ZELDA))
            gSave.entrance = ENTR_OOT_GANON_TOWER;
        break;
    }

    applyCustomEntrance(&gSave.entrance);
}

static void masterSwordFix(PlayState* play)
{
    if (!gSharedCustomSave.foundMasterSword)
        return;

    /* Re-add the Master Sword to the inventory */
    gSave.info.inventory.equipment.swords |= 2;

    if (Config_Flag(CFG_OOT_SWORDLESS_ADULT))
        return;
    if (gSave.age != AGE_ADULT)
        return;
    if (gSave.info.equips.buttonItems[0] != ITEM_NONE)
        return;

    /* We need to force-reequip */
    gSave.info.equips.buttonItems[0] = ITEM_OOT_SWORD_MASTER;
    gSave.info.equips.equipment.swords = 2;
    EV_OOT_UNSET_SWORDLESS();
}

static void Play_AfterInit(PlayState* play)
{
    gLastEntrance = gSave.entrance;
    g.inGrotto = (play->sceneId == SCE_OOT_GROTTOS || play->sceneId == SCE_OOT_FAIRY_FOUNTAIN);
    if (!g.inGrotto)
    {
        gLastScene = play->sceneId;
    }

    /* Spawn Custom Triggers */
    CustomTriggers_Spawn(play);
    comboSpawnCustomWarps(play);

    /* Signs */
    ComboPlay_SpawnExtraSigns(play);

    switch (play->sceneId)
    {
    case SCE_OOT_DEKU_TREE:
    case SCE_OOT_DODONGO_CAVERN:
    case SCE_OOT_INSIDE_JABU_JABU:
    case SCE_OOT_TEMPLE_FOREST:
    case SCE_OOT_TEMPLE_FIRE:
    case SCE_OOT_TEMPLE_WATER:
    case SCE_OOT_TEMPLE_SPIRIT:
    case SCE_OOT_TEMPLE_SHADOW:
    case SCE_OOT_BOTTOM_OF_THE_WELL:
    case SCE_OOT_ICE_CAVERN:
    case SCE_OOT_GANON_TOWER:
    case SCE_OOT_GERUDO_TRAINING_GROUND:
    case SCE_OOT_INSIDE_GANON_CASTLE:
    case SCE_OOT_LAIR_GOHMA:
    case SCE_OOT_LAIR_KING_DODONGO:
    case SCE_OOT_LAIR_BARINADE:
    case SCE_OOT_LAIR_PHANTOM_GANON:
    case SCE_OOT_LAIR_VOLVAGIA:
    case SCE_OOT_LAIR_MORPHA:
    case SCE_OOT_LAIR_BONGO_BONGO:
    case SCE_OOT_LAIR_TWINROVA:
        if (!gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE].playerParams)
        {
            /* Copy to the custom death respawn */
            memcpy(&gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE], &gSaveContext.respawn[RESPAWN_MODE_DOWN], sizeof(OotRespawnData));
            if (gSave.entrance == 0x3f0 || gSave.entrance == 0x3f4)
            {
                /* Copy pos, yaw, playerParams, entrance and roomIndex */
                memcpy(&gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE], &kSpiritTempleMainEntrance, offsetof(OotRespawnData, data));
            }
        }
        break;
    default:
        comboClearCustomRespawn(CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE);
        break;
    }
}

void hookPlay_Init(PlayState* play)
{
    /* Init */
    gIsEntranceOverride = 0;
    g.decoysCount = 0;
    gActorCustomTriggers = NULL;
    gMultiMarkChests = 0;
    gMultiMarkCollectibles = 0;
    gMultiMarkSwitch0 = 0;
    gMultiMarkSwitch1 = 0;
    Multi_ResetWisps();

    /* Register play */
    gPlay = play;

    /* Adjust entrance */
    playAdjustEntrance(play);

    comboCacheClear();
    comboObjectsReset();
    comboExObjectsReset();
    eventFixes(play);
    masterSwordFix(play);

    Play_Init(play);
    Play_AfterInit(play);

    if (!g.customKeep)
    {
        comboLoadCustomKeep();
    }
}

void Play_MainWrapper(PlayState* play)
{
    /* Auto-press A during credits */
    if (g.isCredits)
        play->state.input[0].press.button = (play->state.frameCount & 1) ? A_BUTTON : 0;

    comboMenuTick();
    Debug_Input();
    comboCacheGarbageCollect();
    comboObjectsGC();
    Multi_Update(play);
    Play_Main(play);
    Dpad_Draw(play);
    Audio_DisplayMusicName(play);
    Debug_Update();
}

static void Play_LoadKaleidoScopeHook(void* unk)
{
    Play_LoadKaleidoOverlay(unk);
    comboMqKaleidoHook(gPlay);
}

PATCH_CALL(0x8009a06c, Play_LoadKaleidoScopeHook);

void ReloadSword(PlayState* play);

static void Minigame_Fix(PlayState* play)
{
    int shouldReload;

    shouldReload = 0;
    if (play->sceneId == SCE_OOT_BOMBCHU_BOWLING_ALLEY && Flags_GetSwitch(play, 0x38))
        shouldReload = 1;

    if (shouldReload)
        ReloadSword(play);
}

NORETURN static void Play_GameSwitch(PlayState* play, u32 entrance)
{
    Horse_ForceUnmount(play);
    Minigame_Fix(play);
    comboGameSwitch(play, entrance);
}

static u32 entrGrottoExit(PlayState* play)
{
    if (!Config_Flag(CFG_ER_GROTTOS))
        return ENTR_OOT_INTERNAL_EXIT_GROTTO;

    switch (play->sceneId)
    {
    case SCE_OOT_GROTTOS:
        switch (play->roomCtx.curRoom.num)
        {
        case 0x00:
            switch (gGrottoData & 0x1f)
            {
            case 0x0c: return ENTR_OOT_GROTTO_EXIT_GENERIC_KOKIRI_FOREST;
            case 0x14: return ENTR_OOT_GROTTO_EXIT_GENERIC_LOST_WOODS;
            case 0x08: return ENTR_OOT_GROTTO_EXIT_GENERIC_KAKARIKO;
            case 0x17: return ENTR_OOT_GROTTO_EXIT_GENERIC_DMT;
            case 0x1a: return ENTR_OOT_GROTTO_EXIT_GENERIC_DMC;
            case 0x09: return ENTR_OOT_GROTTO_EXIT_GENERIC_RIVER;
            case 0x02: return ENTR_OOT_GROTTO_EXIT_GENERIC_HF_SOUTHEAST;
            case 0x03: return ENTR_OOT_GROTTO_EXIT_GENERIC_HF_OPEN;
            case 0x00: return ENTR_OOT_GROTTO_EXIT_GENERIC_HF_MARKET;
            }
            UNREACHABLE();
        case 0x01: return ENTR_OOT_GROTTO_EXIT_SCRUB_HEART_PIECE;
        case 0x02: return ENTR_OOT_GROTTO_EXIT_REDEAD;
        case 0x03: return ENTR_OOT_GROTTO_EXIT_TRAIL_COW;
        case 0x04: return ENTR_OOT_GROTTO_EXIT_FIELD_COW;
        case 0x05: return ENTR_OOT_GROTTO_EXIT_OCTOROK;
        case 0x06: return ENTR_OOT_GROTTO_EXIT_SCRUB_UPGRADE;
        case 0x07: return ENTR_OOT_GROTTO_EXIT_WOLFOS;
        case 0x08: return ENTR_OOT_GROTTO_EXIT_CASTLE;
        case 0x09:
            switch (gLastScene)
            {
            case SCE_OOT_SACRED_FOREST_MEADOW: return ENTR_OOT_GROTTO_EXIT_SCRUBS2_SFM;
            case SCE_OOT_ZORA_RIVER: return ENTR_OOT_GROTTO_EXIT_SCRUBS2_RIVER;
            case SCE_OOT_GERUDO_VALLEY: return ENTR_OOT_GROTTO_EXIT_SCRUBS2_VALLEY;
            case SCE_OOT_DESERT_COLOSSUS: return ENTR_OOT_GROTTO_EXIT_SCRUBS2_COLOSSUS;
            }
            UNREACHABLE();
        case 0x0a: return ENTR_OOT_GROTTO_EXIT_TEKTITE;
        case 0x0b: return ENTR_OOT_GROTTO_EXIT_DEKU_THEATER;
        case 0x0c:
            switch (gLastScene)
            {
            case SCE_OOT_LON_LON_RANCH: return ENTR_OOT_GROTTO_EXIT_SCRUBS3_RANCH;
            case SCE_OOT_GORON_CITY: return ENTR_OOT_GROTTO_EXIT_SCRUBS3_GORON_CITY;
            case SCE_OOT_DEATH_MOUNTAIN_CRATER: return ENTR_OOT_GROTTO_EXIT_SCRUBS3_DMC;
            case SCE_OOT_LAKE_HYLIA: return ENTR_OOT_GROTTO_EXIT_SCRUBS3_LAKE;
            }
            UNREACHABLE();
        case 0x0d: return ENTR_OOT_GROTTO_EXIT_FIELD_TREE;
        UNREACHABLE();
        }
        break;
    case SCE_OOT_FAIRY_FOUNTAIN:
        switch (gLastScene)
        {
        case SCE_OOT_SACRED_FOREST_MEADOW: return ENTR_OOT_GROTTO_EXIT_FAIRY_SFM;
        case SCE_OOT_HYRULE_FIELD: return ENTR_OOT_GROTTO_EXIT_FAIRY_HF;
        case SCE_OOT_ZORA_RIVER: return ENTR_OOT_GROTTO_EXIT_FAIRY_RIVER;
        case SCE_OOT_ZORA_DOMAIN: return ENTR_OOT_GROTTO_EXIT_FAIRY_DOMAIN;
        case SCE_OOT_GERUDO_FORTRESS: return ENTR_OOT_GROTTO_EXIT_FAIRY_FORTRESS;
        }
        UNREACHABLE();
    case SCE_OOT_TOMB_FAIRY: return ENTR_OOT_GRAVE_EXIT_SHIELD;
    case SCE_OOT_TOMB_REDEAD: return ENTR_OOT_GRAVE_EXIT_REDEAD;
    case SCE_OOT_TOMB_ROYAL: return ENTR_OOT_GRAVE_EXIT_ROYAL;
    case SCE_OOT_TOMB_DAMPE_WINDMILL: return ENTR_OOT_GRAVE_EXIT_DAMPE;
    }

    return ENTR_OOT_INTERNAL_EXIT_GROTTO;
}

void Play_TransitionDone(PlayState* play)
{
    u32 entrance;
    s32 override;

    /* Resolve extended entrance */
    entrance = play->nextEntranceIndex;
    switch (entrance)
    {
    case ENTR_EXTENDED:
        entrance = g.nextEntrance;
        break;
    case ENTR_FW_CROSS:
        entrance = gSharedCustomSave.mm.fw[gSave.age].entrance | MASK_FOREIGN_ENTRANCE;
        gComboCtx.isFwSpawn = 1;
        break;
    case ENTR_CROSS_RESPAWN:
        entrance = gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE].entrance | MASK_FOREIGN_ENTRANCE;
        gComboCtx.isDungeonEntranceSpawn = 1;
        break;
    }

    /* Handle grottos */
    if (entrance == ENTR_OOT_INTERNAL_EXIT_GROTTO)
    {
        entrance = entrGrottoExit(play);
        if (entrance == ENTR_OOT_INTERNAL_EXIT_GROTTO)
        {
            gIsEntranceOverride = 0;
            entrance = gSaveContext.respawn[1].entrance;
            gSaveContext.respawnFlag = 2;
            gSaveContext.nextTransitionType = 3;
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
        Play_GameSwitch(play, entrance & ~MASK_FOREIGN_ENTRANCE);
    }
    else
    {
        play->nextEntranceIndex = entrance & 0xffff;
    }
}

static void TimeTravelUpdateEquipment(void)
{
    OotItemEquips* prevAge;
    OotItemEquips* nextAge;
    u8 item;

    if (gSave.age == AGE_ADULT)
    {
        prevAge = &gSave.info.adultEquips;
        nextAge = &gSave.info.childEquips;
    }
    else
    {
        prevAge = &gSave.info.childEquips;
        nextAge = &gSave.info.adultEquips;
    }

    memcpy(prevAge, &gSave.info.equips, sizeof(*prevAge));
    if (EV_OOT_IS_SWORDLESS())
        prevAge->buttonItems[0] = ITEM_NONE;
    memcpy(&gSave.info.equips, nextAge, sizeof(*nextAge));

    /* Reload bottles */
    for (int i = 0; i < 3; ++i)
    {
        item = gSave.info.equips.buttonItems[i + 1];
        if (item >= ITEM_OOT_BOTTLE_EMPTY && item <= ITEM_OOT_POE)
            item = gSave.info.inventory.items[gSave.info.equips.cButtonSlots[i]];
        gSave.info.equips.buttonItems[i + 1] = item;
    }

    /* Fix sword */
    if (gSave.info.equips.buttonItems[0] == ITEM_NONE)
        EV_OOT_SET_SWORDLESS();
    else
        EV_OOT_UNSET_SWORDLESS();

    /* Fix shield, if opposite age lost it */
    if (gSave.info.equips.equipment.shields && !(gSave.info.inventory.equipment.shields & (1 << (gSave.info.equips.equipment.shields - 1))))
        gSave.info.equips.equipment.shields = 0;
}

PATCH_FUNC(0x8006f804, TimeTravelUpdateEquipment);

void Play_FastInit(GameState* gs)
{
    u32 entrance;
    u16 magicCapacity;

    /* Normal boot if the game was actually cold-started */
    if (!gComboCtx.valid)
        return;

    /* Enable audio */
    R_AUDIOMGR_DEBUG_LEVEL = 0;

    /* Set the next GameState */
    gs->nextGameStateInit = Play_Init;
    gs->nextGameStateSize = sizeof(PlayState);

    /* Load the save */
    gSaveContext.fileNum = gComboCtx.saveIndex;
    Sram_OpenSave(NULL);

    /* Set game mode */
    gSaveContext.gameMode = GAMEMODE_NORMAL;
    gSaveContext.showTitleCard = TRUE;

    if (gComboCtx.isFwSpawn)
    {
        gSaveContext.respawnFlag = 3;
        gComboCtx.isFwSpawn = 0;

        /* Restore dungeon entrance respawn data. */
        memcpy(&gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE], &gCustomSave.fwRespawnDungeonEntrance[gOotSave.age], sizeof(OotRespawnData));

        OotFaroreWind* fw = &gSave.info.fw;

        if (fw->set)
        {
            gSaveContext.respawn[RESPAWN_MODE_TOP].data = 0x28;
            gSaveContext.respawn[RESPAWN_MODE_TOP].pos.x = fw->pos.x;
            gSaveContext.respawn[RESPAWN_MODE_TOP].pos.y = fw->pos.y;
            gSaveContext.respawn[RESPAWN_MODE_TOP].pos.z = fw->pos.z;
            gSaveContext.respawn[RESPAWN_MODE_TOP].yaw = fw->yaw;
            gSaveContext.respawn[RESPAWN_MODE_TOP].playerParams = fw->playerParams;
            gSaveContext.respawn[RESPAWN_MODE_TOP].entrance = fw->entrance;
            gSaveContext.respawn[RESPAWN_MODE_TOP].roomIndex = fw->roomIndex;
            gSaveContext.respawn[RESPAWN_MODE_TOP].tempSwitchFlags = fw->tempSwitchFlags;
            gSaveContext.respawn[RESPAWN_MODE_TOP].tempCollectFlags = fw->tempCollectFlags;
        }
        else
        {
            gSaveContext.respawn[RESPAWN_MODE_TOP].data = 0;
            gSaveContext.respawn[RESPAWN_MODE_TOP].pos.x = 0.0f;
            gSaveContext.respawn[RESPAWN_MODE_TOP].pos.y = 0.0f;
            gSaveContext.respawn[RESPAWN_MODE_TOP].pos.z = 0.0f;
        }
    }

    if (gComboCtx.isDungeonEntranceSpawn)
    {
        gComboCtx.isDungeonEntranceSpawn = 0;

        if (gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE].playerParams)
        {
            memcpy(&gSaveContext.respawn[RESPAWN_MODE_RETURN], &gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE], sizeof(OotRespawnData));
            gSaveContext.respawnFlag = 2;
        }
    }

    gSave.cutscene = 0;

    /* Set the entrance */
    entrance = gComboCtx.entrance;
    applyCustomEntrance(&entrance);
    gSave.entrance = (s32)entrance;

    /* Set magic */
    magicCapacity = 0;
    if (gSave.info.playerData.isMagicAcquired)
        magicCapacity = gSave.info.playerData.isDoubleMagicAcquired ? 0x60 : 0x30;
    gSaveContext.magicState = MAGIC_STATE_IDLE;
    gSaveContext.magicCapacity = magicCapacity;
    gSaveContext.magicFillTarget = gSave.info.playerData.magic;
    gSaveContext.magicTarget = gSave.info.playerData.magic;

    /* Done */
    gComboCtx.valid = 0;
}
