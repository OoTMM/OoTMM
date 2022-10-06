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
    }
#endif
}

void hookPlay_Init(GameState_Play* play)
{
    comboObjectsReset();
    debugCheat(play);

    Play_Init(play);

    if (gSave.entranceIndex == 0xc010)
    {
        comboGameSwitch();
        return;
    }
}
