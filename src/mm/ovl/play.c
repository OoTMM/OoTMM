#include <combo.h>

GameState_Play* gPlay;
int gNoTimeFlow;

static void debugCheat(GameState_Play* play)
{
#if defined(DEBUG)
    gMiscFlags.erSpring = 1;
    gMiscFlags.erSwampClear = 1;
    gMiscFlags.erCoastClear = 1;
    /*if (play->gs.input[0].current.buttons & 0x20)*/
    {
        gSave.itemEquips.sword = 1;
        gSave.itemEquips.shield = 2;
        gMmExtraFlags2.progressiveShield = 1;
        gSave.itemEquips.buttonItems[0][0] = ITEM_MM_SWORD_KOKIRI;
        gSave.inventory.items[ITS_MM_STICKS] = ITEM_MM_STICK;
        gSave.inventory.items[ITS_MM_OCARINA] = ITEM_MM_OCARINA_OF_TIME;
        gSave.inventory.items[ITS_MM_MASK_DEKU] = ITEM_MM_MASK_DEKU;
        gSave.inventory.items[ITS_MM_MASK_ZORA] = ITEM_MM_MASK_ZORA;
        gSave.inventory.items[ITS_MM_MASK_GORON] = ITEM_MM_MASK_GORON;
        gSave.inventory.items[ITS_MM_BOW] = ITEM_MM_BOW;
        gSave.inventory.items[ITS_MM_LENS] = ITEM_MM_LENS_OF_TRUTH;
        gSave.inventory.items[ITS_MM_BOMBS] = ITEM_MM_BOMB;
        gSave.inventory.items[ITS_MM_HOOKSHOT] = ITEM_MM_HOOKSHOT;
        gSave.inventory.items[ITS_MM_GREAT_FAIRY_SWORD] = ITEM_MM_GREAT_FAIRY_SWORD;
        gSave.inventory.upgrades.quiver = 3;
        gSave.inventory.upgrades.wallet = 2;
        gSave.inventory.upgrades.bombBag = 3;
        gSave.playerData.rupees = 500;
        gSave.inventory.quest.songHealing = 1;
        gSave.inventory.quest.songTime = 1;
        gSave.inventory.quest.songSoaring = 1;
        gSave.inventory.quest.songEpona = 1;
        gSave.inventory.quest.songOrder = 1;
        gSave.inventory.quest.songStorms = 1;
        gSave.inventory.quest.songNewWave = 1;
        gSave.inventory.quest.songAwakening = 1;
        gSave.inventory.items[ITS_MM_BOTTLE + 0] = ITEM_MM_BOTTLED_DEKU_PRINCESS;
        gSave.inventory.items[ITS_MM_BOTTLE + 1] = ITEM_MM_BOTTLED_SPRING_WATER_HOT;
        gSave.playerData.magicAmount = 0x30;
        gMmSave.playerData.magicAcquired = 1;
        gSaveContext.magicTarget = 0x30;
        gSave.inventory.items[ITS_MM_MASK_GORON] = ITEM_MM_MASK_GORON;
        gSave.inventory.ammo[ITS_MM_STICKS] = 10;
        gSave.inventory.ammo[ITS_MM_BOW] = 50;
        gSave.inventory.ammo[ITS_MM_BOMBS] = 40;
        gSave.inventory.items[ITS_MM_ARROW_FIRE] = ITEM_MM_ARROW_FIRE;
        gSave.inventory.items[ITS_MM_ARROW_LIGHT] = ITEM_MM_ARROW_LIGHT;
        gSave.inventory.items[ITS_MM_PICTOBOX] = ITEM_MM_PICTOGRAPH_BOX;
        gSave.inventory.quest.songLullabyIntro = 1;
        gSave.inventory.quest.songLullaby = 1;

        gSave.inventory.items[ITS_MM_KEG] = ITEM_MM_POWDER_KEG;
        gSave.inventory.items[ITS_MM_MASK_CAPTAIN] = ITEM_MM_MASK_CAPTAIN;
        gSave.playerData.healthMax = 0x10 * 10;
        gSave.playerData.health = gSave.playerData.healthMax;

        gSave.inventory.items[ITS_MM_TRADE1] = ITEM_MM_DEED_LAND;
        gSave.inventory.items[ITS_MM_TRADE3] = ITEM_MM_LETTER_TO_KAFEI;
        gMmExtraTrade.trade1 = 0x02;
        gMmExtraTrade.trade2 = 0x03;
        gMmExtraTrade.trade3 = 0x03;

        gSave.inventory.items[ITS_MM_TRADE2] = ITEM_MM_ROOM_KEY;
        gSave.inventory.items[ITS_MM_TRADE3] = ITEM_MM_PENDANT_OF_MEMORIES;

        gSave.inventory.items[ITS_MM_MASK_KAFEI] = ITEM_MM_MASK_KAFEI;
        gSave.inventory.items[ITS_MM_MASK_ROMANI] = ITEM_MM_MASK_ROMANI;
        gSave.inventory.items[ITS_MM_MASK_COUPLE] = ITEM_MM_MASK_COUPLE;
        gSave.inventory.items[ITS_MM_MASK_POSTMAN] = ITEM_MM_MASK_POSTMAN;
        gSave.inventory.items[ITS_MM_MASK_TROUPE_LEADER] = ITEM_MM_MASK_TROUPE_LEADER;
        gSave.inventory.items[ITS_MM_MASK_FIERCE_DEITY] = ITEM_MM_MASK_FIERCE_DEITY;

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
        gSave.entranceIndex = g.initialEntrance;
        comboReadForeignSave();
    }

    if (gSave.entranceIndex == 0xd800 && gLastEntrance == 0x1c00)
    {
        /* Song of Time */
        gSave.entranceIndex = g.initialEntrance;
    }

    if (gSave.entranceIndex == 0x8610)
    {
        /* Woodfall from temple */
        if (!MM_GET_EVENT_WEEK(EV_MM_WEEK_WOODFALL_TEMPLE_RISE))
        {
            gSave.entranceIndex = 0x8640;
        }
    }

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
        if (!comboGoalCond())
        {
            gSave.playerForm = MM_PLAYER_FORM_HUMAN;
            gSave.equippedMask = 0;
            gSave.day = 0;
            gSave.time = 0x3fff;
            Sram_SaveNewDay(play);
            play->nextEntrance = g.initialEntrance;
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
    comboObjectsGC();
    Play_Draw(play);
    comboDpadDraw(play);
}
