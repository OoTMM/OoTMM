#include <combo.h>
#include <combo/item.h>

static int EnOssan_HasGivenItem(Actor_EnOssan* this, GameState_Play* play)
{
    if (!Actor_HasParent(&this->base) || Message_GetState(&play->msgCtx) != 6)
        return 0;

    comboShopAfterBuy(play, this->items[this->itemIndex]);
    this->base.attachedA = NULL;
    for (int i = 0; i < 8; ++i)
        comboShopUpdateItem(play, this->items[i]);
    return 1;
}

PATCH_CALL(0x808d1cac, EnOssan_HasGivenItem);

static void EnOssan_GiveItem(Actor_EnOssan* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Actor_EnGirlA* girlA;

    girlA = this->items[this->itemIndex];
    EnGirlA_ItemQuery(&q, girlA);
    comboGiveItem(&this->base, play, &q, a, b);
}

PATCH_CALL(0x808d1cf0, EnOssan_GiveItem);
