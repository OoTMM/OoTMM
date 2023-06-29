#include <combo.h>
#include <combo/item.h>

static int EnTrt_HasGivenItem(Actor_EnTrt* this, GameState_Play* play)
{
    if (!Actor_HasParent(&this->base))
        return 0;

    comboShopAfterBuy(play, this->items[this->itemIndex]);
    return 1;
}

PATCH_CALL(0x80a8dfcc, EnTrt_HasGivenItem);

static void EnTrt_DisplayShopTextBoxConfirm(Actor_EnTrt* this)
{
    comboShopDisplayTextBoxConfirm(gPlay, this->items[this->itemIndex]);
}

PATCH_FUNC(0x80a8bb3c, EnTrt_DisplayShopTextBoxConfirm);

static void EnTrt_DisplayShopTextBox(Actor_EnTrt* this)
{
    comboShopDisplayTextBox(gPlay, this->items[this->itemIndex]);
}

PATCH_FUNC(0x80a8baf8, EnTrt_DisplayShopTextBox);

void EnTrt_GiveItem_BottledRedPotion(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(actor, play, gi, NPC_MM_KOTAKE_RED_POTION, a, b);
}

PATCH_CALL(0x80a8c54c, EnTrt_GiveItem_BottledRedPotion);
PATCH_CALL(0x80ad4094, EnTrt_GiveItem_BottledRedPotion); /* En_Trt2 */

static void EnTrt_GiveItem(Actor_EnTrt* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Actor_EnGirlA* girlA;

    girlA = this->items[this->itemIndex];
    EnGirlA_ItemQuery(&q, girlA);
    comboGiveItem(&this->base, play, &q, a, b);
}

PATCH_CALL(0x80a8cc44, EnTrt_GiveItem);
PATCH_CALL(0x80a8e014, EnTrt_GiveItem);
