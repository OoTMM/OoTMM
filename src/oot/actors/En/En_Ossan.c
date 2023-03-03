#include <combo.h>

void comboAfterBuy(Actor_EnGirlA* girlA, GameState_Play* play)
{
    girlA->gi = GI_MM_SOLD_OUT | MASK_FOREIGN_GI;
}

static int EnOssan_HasGivenItem(Actor_EnOssan* this, GameState_Play* play)
{
    if (!Actor_HasParent(&this->base))
        return 0;

    comboAfterBuy(this->items[this->itemIndex], play);
    this->base.attachedA = NULL;
    return 1;
}

PATCH_CALL(0x808d1cac, EnOssan_HasGivenItem);
