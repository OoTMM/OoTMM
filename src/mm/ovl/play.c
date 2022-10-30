#include <combo.h>

extern void Play_Init(GameState_Play*);

static void debugCheat(GameState_Play* play)
{
#if defined(DEBUG)
    /*if (play->gs.input[0].current.buttons & 0x20)*/
    {
        gSave.itemEquips.sword = 3;
        gSave.itemEquips.shield = 2;
        gSave.itemEquips.buttonItems[0][0] = ITEM_MM_SWORD_GILDED;
        gSave.inventory.items[ITS_MM_OCARINA] = ITEM_MM_OCARINA_OF_TIME;
        gSave.inventory.items[ITS_MM_MASK_DEKU] = ITEM_MM_MASK_DEKU;
        gSave.inventory.items[ITS_MM_MASK_ZORA] = ITEM_MM_MASK_ZORA;
        gSave.inventory.items[ITS_MM_BOW] = ITEM_MM_BOW;
        gSave.inventory.upgrades.quiver = 3;
        gSave.inventory.upgrades.wallet = 2;
        gSave.playerData.rupees = 500;
        gSave.inventory.questItems.songHealing = 1;
        gSave.inventory.items[ITS_MM_BOTTLE + 0] = ITEM_MM_BOTTLED_GOLD_DUST;
        gSave.inventory.items[ITS_MM_BOTTLE + 1] = ITEM_MM_BOTTLED_SPRING_WATER_HOT;
    }
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
