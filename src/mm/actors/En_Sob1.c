#include <combo.h>

void EnSob1_AfterBuy(Actor_EnSob1* sob1, GameState_Play* play)
{
    if (HasActorGivenItem(&sob1->base))
    {
        comboAfterBuy(sob1->items[sob1->itemIndex], play);
        sob1->base.attachedA = NULL;
    }
}
