#include <combo.h>

void Interface_UpdateButtonsPart2Wrapper(GameState_Play* play)
{
    u8* ptr;
    u8 itemId;
    u8 buttons[3];

    for (int i = 0; i < 3; ++i)
    {
        ptr = &gSave.itemEquips.buttonItems[0][i + 1];
        itemId = *ptr;
        buttons[i] = itemId;

        if (itemId == ITEM_MM_OCARINA_FAIRY)
            *ptr = ITEM_MM_OCARINA_OF_TIME;
    }

    Interface_UpdateButtonsPart2(play);

    for (int i = 0; i < 3; ++i)
    {
        ptr = &gSave.itemEquips.buttonItems[0][i + 1];
        *ptr = buttons[i];
    }
}
