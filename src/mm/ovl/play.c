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
        gSave.inventory.questItems.songHealing = 1;
        gSave.inventory.items[ITS_MM_BOTTLE + 0] = ITEM_MM_BOTTLED_ZORA_EGG;
        gSave.inventory.items[ITS_MM_BOTTLE + 1] = ITEM_MM_BOTTLED_ZORA_EGG;
        gSave.inventory.items[ITS_MM_BOTTLE + 2] = ITEM_MM_BOTTLED_ZORA_EGG;
        gSave.inventory.items[ITS_MM_BOTTLE + 3] = ITEM_MM_BOTTLED_ZORA_EGG;
    }
#endif
}

void hookPlay_Init(GameState_Play* play)
{
    comboObjectsReset();
    debugCheat(play);

    /* Force alt beaver race */
    MM_SET_EVENT_WEEK(EV_MM_WEEK_BEAVER_RACE_ALT);

    Play_Init(play);

    if (gSave.entranceIndex == 0xc010)
    {
        gSave.isOwlSave = 1;
        PrepareSave(&play->sramCtx);
        comboGameSwitch();
        return;
    }
}
