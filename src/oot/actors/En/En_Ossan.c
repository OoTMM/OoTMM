#include <combo.h>

static int EnOssan_HasGivenItem(Actor_EnOssan* this, GameState_Play* play)
{
    if (!Actor_HasParent(&this->base))
        return 0;

    comboShopAfterBuy(play, this->items[this->itemIndex]);
    this->base.attachedA = NULL;
    for (int i = 0; i < 8; ++i)
        comboShopUpdateItem(play, this->items[i]);
    return 1;
}

PATCH_CALL(0x808d1cac, EnOssan_HasGivenItem);
