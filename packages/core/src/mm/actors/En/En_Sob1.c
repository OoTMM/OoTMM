#include <combo.h>

void EnSob1_AfterBuy(Actor_EnSob1* sob1, GameState_Play* play)
{
    if (Actor_HasParent(&sob1->base))
    {
        comboShopAfterBuy(play, sob1->items[sob1->itemIndex]);
        sob1->base.attachedA = NULL;
    }
}
