#include <combo.h>
#include <combo/net.h>
#include <combo/menu.h>
#include <combo/entrance.h>
#include <combo/time.h>

GameState_Play* gPlay;
int gNoTimeFlow;

static void debugCheat(GameState_Play* play)
{
#if defined(DEBUG)
    //if (play->gs.input[0].current.buttons & L_TRIG)
    {
        gSave.itemEquips.sword = 1;
        gSave.itemEquips.shield = 2;
        gMmExtraFlags2.progressiveShield = 1;
        gSave.inventory.upgrades.quiver = 3;
        gSave.inventory.upgrades.dekuStick = 3;
        gSave.inventory.upgrades.dekuNut = 3;
        gSave.inventory.upgrades.strength = 3;
        //gMmExtraFlags2.childWallet = 1;
        //gSave.inventory.upgrades.wallet = 3;
        //gMmExtraFlags3.bottomlessWallet = 1;
        gSave.inventory.upgrades.bombBag = 0;

        gSave.itemEquips.buttonItems[0][0] = ITEM_MM_SWORD_KOKIRI;
        gSave.inventory.items[ITS_MM_OCARINA] = ITEM_MM_OCARINA_OF_TIME;
        gSave.inventory.items[ITS_MM_BOW] = ITEM_MM_BOW;
        gSave.inventory.items[ITS_MM_ARROW_FIRE] = ITEM_MM_ARROW_FIRE;
        gSave.inventory.items[ITS_MM_ARROW_ICE] = ITEM_MM_ARROW_ICE;
        gSave.inventory.items[ITS_MM_ARROW_LIGHT] = ITEM_MM_ARROW_LIGHT;
        gSave.inventory.items[ITS_MM_BOMBS] = ITEM_MM_BOMB;
        gSave.inventory.items[ITS_MM_BOMBCHU] = ITEM_MM_BOMBCHU;
        gSave.inventory.items[ITS_MM_STICKS] = ITEM_MM_STICK;
        gSave.inventory.items[ITS_MM_NUTS] = ITEM_MM_NUT;
        gSave.inventory.items[ITS_MM_BEANS] = ITEM_MM_MAGIC_BEAN;
        gSave.inventory.items[ITS_MM_KEG] = ITEM_MM_POWDER_KEG;
        gSave.inventory.items[ITS_MM_PICTOBOX] = ITEM_MM_PICTOGRAPH_BOX;
        gSave.inventory.items[ITS_MM_LENS] = ITEM_MM_LENS_OF_TRUTH;
        gSave.inventory.items[ITS_MM_HOOKSHOT] = ITEM_MM_HOOKSHOT;
        gSave.inventory.items[ITS_MM_GREAT_FAIRY_SWORD] = ITEM_MM_GREAT_FAIRY_SWORD;
        gSave.inventory.items[ITS_MM_BOTTLE + 0] = ITEM_MM_DEKU_PRINCESS;
        gSave.inventory.items[ITS_MM_BOTTLE + 1] = ITEM_MM_SPRING_WATER_HOT;
        gSave.inventory.items[ITS_MM_BOTTLE + 2] = ITEM_MM_MAGIC_MUSHROOM;
        gSave.inventory.items[ITS_MM_BOTTLE + 3] = ITEM_MM_POE;
        gSave.inventory.items[ITS_MM_BOTTLE + 4] = ITEM_MM_BOTTLE_EMPTY;
        gSave.inventory.items[ITS_MM_BOTTLE + 5] = ITEM_MM_BOTTLE_EMPTY;

        gSave.inventory.items[ITS_MM_STICKS] = ITEM_MM_STICK;
        gSave.inventory.ammo[ITS_MM_STICKS] = 30;

        gSave.inventory.items[ITS_MM_MASK_POSTMAN] = ITEM_MM_MASK_POSTMAN;
        gSave.inventory.items[ITS_MM_MASK_ALL_NIGHT] = ITEM_MM_MASK_ALL_NIGHT;
        gSave.inventory.items[ITS_MM_MASK_BLAST] = ITEM_MM_MASK_BLAST;
        gSave.inventory.items[ITS_MM_MASK_STONE] = ITEM_MM_MASK_STONE;
        gSave.inventory.items[ITS_MM_MASK_GREAT_FAIRY] = ITEM_MM_MASK_GREAT_FAIRY;
        gSave.inventory.items[ITS_MM_MASK_DEKU] = ITEM_MM_MASK_DEKU;
        gSave.inventory.items[ITS_MM_MASK_KEATON] = ITEM_MM_MASK_KEATON;
        gSave.inventory.items[ITS_MM_MASK_BREMEN] = ITEM_MM_MASK_BREMEN;
        gSave.inventory.items[ITS_MM_MASK_BUNNY] = ITEM_MM_MASK_BUNNY;
        gSave.inventory.items[ITS_MM_MASK_DON_GERO] = ITEM_MM_MASK_DON_GERO;
        gSave.inventory.items[ITS_MM_MASK_SCENTS] = ITEM_MM_MASK_SCENTS;
        gSave.inventory.items[ITS_MM_MASK_GORON] = ITEM_MM_MASK_GORON;
        gSave.inventory.items[ITS_MM_MASK_ROMANI] = ITEM_MM_MASK_ROMANI;
        gSave.inventory.items[ITS_MM_MASK_TROUPE_LEADER] = ITEM_MM_MASK_TROUPE_LEADER;
        gSave.inventory.items[ITS_MM_MASK_KAFEI] = ITEM_MM_MASK_KAFEI;
        gSave.inventory.items[ITS_MM_MASK_COUPLE] = ITEM_MM_MASK_COUPLE;
        gSave.inventory.items[ITS_MM_MASK_TRUTH] = ITEM_MM_MASK_TRUTH;
        gSave.inventory.items[ITS_MM_MASK_ZORA] = ITEM_MM_MASK_ZORA;
        gSave.inventory.items[ITS_MM_MASK_KAMARO] = ITEM_MM_MASK_KAMARO;
        gSave.inventory.items[ITS_MM_MASK_GIBDO] = ITEM_MM_MASK_GIBDO;
        gSave.inventory.items[ITS_MM_MASK_GARO] = ITEM_MM_MASK_GARO;
        gSave.inventory.items[ITS_MM_MASK_CAPTAIN] = ITEM_MM_MASK_CAPTAIN;
        gSave.inventory.items[ITS_MM_MASK_GIANT] = ITEM_MM_MASK_GIANT;
        gSave.inventory.items[ITS_MM_MASK_FIERCE_DEITY] = ITEM_MM_MASK_FIERCE_DEITY;

        gSave.playerData.rupees = 9999;

        gSave.inventory.quest.notebook = 1;
        gSave.inventory.quest.songHealing = 1;
        gSave.inventory.quest.songTime = 1;
        gSave.inventory.quest.songSoaring = 1;
        gSave.inventory.quest.songEpona = 1;
        gSave.inventory.quest.songStorms = 1;
        gSave.inventory.quest.songAwakening = 1;
        gSave.inventory.quest.songLullabyIntro = 1;
        gSave.inventory.quest.songLullaby = 1;
        gSave.inventory.quest.songNewWave = 1;
        gSave.inventory.quest.songEmpty = 1;
        gSave.inventory.quest.songOrder = 1;
        gSave.inventory.quest.songSun = 1;
        gSave.inventory.quest.songSaria = 0; // left out because it currently breaks the audio channels

        gMmSave.playerData.magicAcquired = 1;
        gMmSave.playerData.doubleMagic = 1;
        gMmSave.playerData.magicAmount = 2 * 0x30;
        gSaveContext.magicFillTarget = 0x60;

        gSave.inventory.ammo[ITS_MM_STICKS] = 30;
        gSave.inventory.ammo[ITS_MM_NUTS] = 40;
        gSave.inventory.ammo[ITS_MM_KEG] = 1;
        gSave.inventory.ammo[ITS_MM_BEANS] = 10;
        gSave.inventory.ammo[ITS_MM_BOW] = 50;
        gSave.inventory.ammo[ITS_MM_BOMBCHU] = 50;
        gSave.inventory.ammo[ITS_MM_BOMBS] = 40;

        gSave.playerData.healthMax = 0x10 * 20;
        gSave.playerData.health = gSave.playerData.healthMax;

        gMmExtraTrade.trade1 = 0x3f;
        gMmExtraTrade.trade2 = 0x1f;
        gMmExtraTrade.trade3 = 0x1f;
        gMmExtraItems.hookshot = 3;
        gMmExtraItems.ocarina = 3;

        gSave.inventory.items[ITS_MM_TRADE1] = ITEM_MM_DEED_LAND;
        gSave.inventory.items[ITS_MM_TRADE2] = ITEM_MM_ROOM_KEY;
        gSave.inventory.items[ITS_MM_TRADE3] = ITEM_MM_PENDANT_OF_MEMORIES;

        gMmOwlFlags = 0x3ff; // all owls statues
        //gSave.inventory.quest.remainsOdolwa = 1;
        //gMmExtraBoss.boss |= 0x01;

        //gSave.day = 3;
        //gSave.isNight = 1;
        //gSave.time = 0x3000;

        //gSave.skullCountOcean = 0x10;
    }

#endif
}

static int sTimeSkipFlag;

static void checkTimeSkip(GameState_Play* play)
{
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
            Play_SetupRespawnPoint(play, 1, 0xdff);
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
    switch (entrance)
    {
    case 0x0c00:
        /* To Clear Swamp from road */
        return ENTR_MM_SWAMP_FROM_ROAD;
    case 0xae60:
        /* To Spring Mountain Village from Path */
        return ENTR_MM_MOUNTAIN_VILLAGE_FROM_PATH;
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

void preInitTitleScreen(void)
{
    if (!gComboCtx.valid)
        return;

    /* Disable Title screen */
    gSaveContext.gameMode = 0;

    /* Load save */
    gSaveContext.fileIndex = gComboCtx.saveIndex;
    Sram_OpenSave(NULL, NULL);

    gSave.cutscene = 0;
    gSaveContext.nextCutscene = 0;
    if (gComboCtx.entrance == -1)
        gSave.entranceIndex = ENTR_MM_CLOCK_TOWN;
    else
        gSave.entranceIndex = gComboCtx.entrance;
    if (comboConfig(CFG_ER_ANY))
        g.initialEntrance = gSave.entranceIndex;
    else
        g.initialEntrance = ENTR_MM_CLOCK_TOWN;

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
    int isEndOfGame;

    /* Pre-init */
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
        gSave.entranceIndex = entranceForOverride(g.initialEntrance);
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
    debugCheat(play);

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
    gPlay = play;
    gLastEntrance = gSave.entranceIndex;
    g.inGrotto = (play->sceneId == SCE_MM_GROTTOS);
    if (!g.inGrotto)
    {
        gLastScene = play->sceneId;
    }
    CustomTriggers_Spawn(play);
    comboSpawnItemGivers(play);
    comboSpawnCustomWarps(play);

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
            play->nextEntrance = entranceForOverride(g.initialEntrance);
            play->transitionTrigger = TRANS_TRIGGER_NORMAL;
            play->transitionType = TRANS_TYPE_FADE_BLACK;
            return;
        }
    }

    if (gSave.entranceIndex == ENTR_MM_CLOCK_TOWER_FROM_CLOCK_TOWN)
    {
        comboGameSwitch(play, -1);
        return;
    }
}

void Play_UpdateWrapper(GameState_Play* play)
{
    comboMenuTick();
    Debug_Input();
    malloc_check();
    comboCacheGarbageCollect();
    comboObjectsGC();
    checkTimeSkip(play);
    Play_Update(play);
    Debug_Update();
}

NORETURN static void Play_GameSwitch(GameState_Play* play, s32 entrance)
{
    comboGameSwitch(play, entrance);
}

void Play_TransitionDone(GameState_Play* play)
{
    u32 entrance;
    s32 override;

    /* Resolve extended entrance */
    entrance = play->nextEntrance;
    if (entrance == ENTR_EXTENDED)
        entrance = g.nextEntrance;

    /* Handle transition override */
    if (g.inGrotto)
        gIsEntranceOverride = 0;
    if (gIsEntranceOverride)
    {
        gIsEntranceOverride = 0;
        override = comboEntranceOverride(entranceForOverride(entrance));
        if (override != -1)
            entrance = (u32)override;
    }

    /* Check for foreign */
    if (entrance & MASK_FOREIGN_ENTRANCE)
    {
        Play_GameSwitch(play, entrance & ~MASK_FOREIGN_ENTRANCE);
    }
    else
    {
        play->nextEntrance = entrance & 0xffff;
    }
}
