#include <combo.h>
#include <combo/item.h>
#include <combo/shop.h>

void EnSob1_AfterBuy(Actor_EnSob1* this, GameState_Play* play)
{
    if (!Actor_HasParentZ(&this->base))
        return;

    Shop_AfterBuy(play, this->items[this->itemIndex]);
    this->base.parent = NULL;
}

static void EnSob1_GiveItem(Actor_EnSob1* sob1, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Actor_EnGirlA* girlA;

    girlA = sob1->items[sob1->itemIndex];
    EnGirlA_ItemQuery(&q, girlA);
    comboGiveItem(&sob1->base, play, &q, a, b);
}

PATCH_CALL(0x80a0e468, EnSob1_GiveItem);
PATCH_CALL(0x80a0eae0, EnSob1_GiveItem);
