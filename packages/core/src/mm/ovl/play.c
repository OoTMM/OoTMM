#include <combo.h>

GameState_Play* gPlay;
int gNoTimeFlow;

static void debugCheat(GameState_Play* play)
{
#if defined(DEBUG)
    //if (play->gs.input[0].current.buttons & L_TRIG)
    {
        gSave.itemEquips.sword = 3;
        gSave.itemEquips.shield = 2;
        gMmExtraFlags2.progressiveShield = 1;
        gSave.inventory.upgrades.quiver = 3;
        gSave.inventory.upgrades.dekuStick = 3;
        gSave.inventory.upgrades.dekuNut = 3;
        gMmExtraFlags2.childWallet = 1;
        gSave.inventory.upgrades.wallet = 3;
        gMmExtraFlags3.bottomlessWallet = 1;
        gSave.inventory.upgrades.bombBag = 3;

        gSave.itemEquips.buttonItems[0][0] = ITEM_MM_SWORD_GILDED;
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
        gSave.inventory.items[ITS_MM_BOTTLE + 2] = ITEM_MM_BOTTLE_EMPTY;
        gSave.inventory.items[ITS_MM_BOTTLE + 3] = ITEM_MM_BOTTLE_EMPTY;
        gSave.inventory.items[ITS_MM_BOTTLE + 4] = ITEM_MM_BOTTLE_EMPTY;
        gSave.inventory.items[ITS_MM_BOTTLE + 5] = ITEM_MM_BOTTLE_EMPTY;

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
        gSave.inventory.ammo[ITS_MM_BOMBCHU] = 40;
        gSave.inventory.ammo[ITS_MM_BOMBS] = 40;

        gSave.playerData.healthMax = 0x10 * 20;
        gSave.playerData.health = gSave.playerData.healthMax;

        gMmExtraTrade.trade1 = 0x1f;
        gMmExtraTrade.trade2 = 0x03;
        gMmExtraTrade.trade3 = 0x03;

        gSave.inventory.items[ITS_MM_TRADE1] = ITEM_MM_DEED_LAND;
        gSave.inventory.items[ITS_MM_TRADE2] = ITEM_MM_ROOM_KEY;
        gSave.inventory.items[ITS_MM_TRADE3] = ITEM_MM_PENDANT_OF_MEMORIES;

        gMmOwlFlags = 0b001111111111; // all owls statues
        //gSave.inventory.quest.remainsOdolwa = 1;
        //gMmExtraBoss.boss |= 0x01;

        //gSave.day = 3;
        //gSave.isNight = 1;
        //gSave.time = 0x3000;

        //gSave.skullCountOcean = 0x10;
    }

#endif
}

static u32 entranceForOverride(u32 entrance)
{
    switch (entrance)
    {
    case 0x0c00:
        /* To Clear Swamp from road */
        return 0x8400;
    case 0xae60:
        /* To Spring Mountain Village from Path */
        return 0x9a60;
    default:
        return entrance;
    }
}

void hookPlay_Init(GameState_Play* play)
{
    int isEndOfGame;
    s32 override;

    isEndOfGame = 0;

    /* Init */
    gActorCustomTriggers = NULL;
    g.customItemsList = NULL;
    g.keatonGrassMax = -1;

    /* Handle transition override */
    if (g.inGrotto)
        gIsEntranceOverride = 0;
    if (gIsEntranceOverride)
    {
        gIsEntranceOverride = 0;
        override = comboEntranceOverride(entranceForOverride(gSave.entranceIndex));
        if (override != -1)
        {
            if (override >= 0)
                gSave.entranceIndex = override;
            else
            {
                gSave.entranceIndex = gLastEntrance;
                Play_Init(play);
                comboGameSwitch(play, override);
                return;
            }
        }
    }

    if (!gCustomKeep)
    {
        comboLoadCustomKeep();
    }

    if (gSave.entranceIndex == 0x5400 && gSaveContext.nextCutscene == 0xfff7)
    {
        isEndOfGame = 1;
    }

    if (gSave.entranceIndex == 0xc030)
    {
        /* Moon crash */
        gSave.entranceIndex = entranceForOverride(g.initialEntrance);
        comboReadForeignSave();
    }

    if (gSave.entranceIndex == 0xd800 && gLastEntrance == 0x1c00)
    {
        /* Song of Time */
        gSave.entranceIndex = entranceForOverride(g.initialEntrance);
    }

    if (gSave.entranceIndex == 0x8610)
    {
        /* Woodfall from temple */
        if (!MM_GET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE))
        {
            gSave.entranceIndex = 0x8640;
        }
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
            play->transitionType = TRANS_TYPE_BLACK;
            return;
        }
    }

    if (gSave.entranceIndex == 0xc010)
    {
        comboGameSwitch(play, -1);
        return;
    }
}

void Play_DrawWrapper(GameState_Play* play)
{
    comboCacheGarbageCollect();
    comboObjectsGC();
    Play_Draw(play);
    comboDpadDraw(play);
}
