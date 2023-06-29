#include <combo.h>
#include <combo/item.h>

static int EnOssan_HasGiveItem(Actor_EnOssan* this, GameState_Play* play)
{
    if (!Actor_HasParent(&this->base))
        return 0;

    comboShopAfterBuy(play, this->items[this->itemIndex]);
    return 1;
}

PATCH_CALL(0x808aa2d8, EnOssan_HasGiveItem);

static void EnOssan_GiveItem(Actor_EnOssan* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Actor_EnGirlA* girlA;

    girlA = this->items[this->itemIndex];
    EnGirlA_ItemQuery(&q, girlA);
    comboGiveItem(&this->base, play, &q, a, b);
}

PATCH_CALL(0x808a9dac, EnOssan_GiveItem);
PATCH_CALL(0x808aa324, EnOssan_GiveItem);
