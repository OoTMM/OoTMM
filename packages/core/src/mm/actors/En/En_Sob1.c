#include <combo.h>
#include <combo/item.h>

void EnSob1_AfterBuy(Actor_EnSob1* sob1, GameState_Play* play)
{
    if (Actor_HasParent(&sob1->base))
    {
        comboShopAfterBuy(play, sob1->items[sob1->itemIndex]);
        sob1->base.attachedA = NULL;
    }
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
