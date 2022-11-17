#include <combo.h>

extern void Play_Init(GameState_Play*);

static void debugCheat(GameState_Play* play)
{
#if defined(DEBUG)
#if 0
    /*if (play->gs.input[0].current.buttons & 0x20)*/
    {
        gSave.itemEquips.sword = 3;
        gSave.itemEquips.shield = 2;
        gSave.itemEquips.buttonItems[0][0] = ITEM_MM_SWORD_GILDED;
        gSave.inventory.items[ITS_MM_OCARINA] = ITEM_MM_OCARINA_OF_TIME;
        gSave.inventory.items[ITS_MM_MASK_DEKU] = ITEM_MM_MASK_DEKU;
        gSave.inventory.items[ITS_MM_MASK_ZORA] = ITEM_MM_MASK_ZORA;
        gSave.inventory.items[ITS_MM_MASK_GORON] = ITEM_MM_MASK_GORON;
        //gSave.inventory.items[ITS_MM_BOW] = ITEM_MM_BOW;
        gSave.inventory.items[ITS_MM_LENS] = ITEM_MM_LENS_OF_TRUTH;
        //gSave.inventory.upgrades.quiver = 3;
        gSave.inventory.upgrades.wallet = 2;
        gSave.inventory.upgrades.bombBag = 3;
        gSave.playerData.rupees = 500;
        gSave.inventory.questItems.songHealing = 1;
        gSave.inventory.questItems.songTime = 1;
        gSave.inventory.questItems.songSoaring = 1;
        gSave.inventory.questItems.songEpona = 1;
        gSave.inventory.items[ITS_MM_BOTTLE + 0] = ITEM_MM_BOTTLED_GOLD_DUST;
        gSave.inventory.items[ITS_MM_BOTTLE + 1] = ITEM_MM_BOTTLED_SPRING_WATER_HOT;
        gSave.playerData.magic = 0x30;
        gMmSave.playerData.magicAcquired = 1;
        gSaveContext.magicTarget = 0x30;
        gSave.inventory.items[ITS_MM_MASK_GORON] = ITEM_MM_MASK_GORON;
        gSave.inventory.ammo[ITS_MM_BOW] = 50;
        gSave.inventory.items[ITS_MM_ARROW_FIRE] = ITEM_MM_ARROW_FIRE;
        gSave.inventory.questItems.songLullabyIntro = 1;

        gSave.inventory.items[ITS_MM_KEG] = ITEM_MM_POWDER_KEG;
        gSave.inventory.items[ITS_MM_MASK_CAPTAIN] = ITEM_MM_MASK_CAPTAIN;
        gSave.playerData.healthCapacity = 0x10 * 20;
        gSave.playerData.health = gSave.playerData.healthCapacity;

        gSave.inventory.items[ITS_MM_TRADE1] = ITEM_MM_DEED_LAND;
        gSave.inventory.items[ITS_MM_TRADE3] = ITEM_MM_LETTER_TO_KAFEI;
        gMmExtraTrade.trade1 = 0x02;
        gMmExtraTrade.trade2 = 0xff;
        gMmExtraTrade.trade3 = 0xff;

        gSave.inventory.items[ITS_MM_TRADE2] = ITEM_MM_ROOM_KEY;
        gSave.inventory.items[ITS_MM_TRADE3] = ITEM_MM_PENDANT_OF_MEMORIES;

        gSave.inventory.items[ITS_MM_MASK_KAFEI] = ITEM_MM_MASK_KAFEI;
        gSave.inventory.items[ITS_MM_MASK_ROMANI] = ITEM_MM_MASK_ROMANI;
        gSave.inventory.items[ITS_MM_MASK_COUPLE] = ITEM_MM_MASK_COUPLE;
        gSave.inventory.items[ITS_MM_MASK_POSTMAN] = ITEM_MM_MASK_POSTMAN;
        gSave.inventory.items[ITS_MM_MASK_TROUPE_LEADER] = ITEM_MM_MASK_TROUPE_LEADER;

        //gSave.inventory.questItems.remainsOdolwa = 1;
        gMmExtraBoss |= 0x01;
    }
#endif
    gSave.inventory.questItems.songTime = 1;
    gSave.inventory.items[ITS_MM_OCARINA] = ITEM_MM_OCARINA_OF_TIME;
#endif
}

void hookPlay_Init(GameState_Play* play)
{
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

    if (gSave.entranceIndex == 0xc010)
    {
        gSave.isOwlSave = 1;
        PrepareSave(&play->sramCtx);
        comboGameSwitch();
        return;
    }
}
