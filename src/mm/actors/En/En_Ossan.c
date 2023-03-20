#include <combo.h>

static int EnOssan_HasGiveItem(Actor_EnOssan* this, GameState_Play* play)
{
    if (!Actor_HasParent(&this->base))
        return 0;

    comboShopAfterBuy(play, this->items[this->itemIndex]);
    return 1;
}

PATCH_CALL(0x808aa2d8, EnOssan_HasGiveItem);
