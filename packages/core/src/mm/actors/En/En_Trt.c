#include <combo.h>

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

int EnTrt_GiveItem_BottledRedPotion(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_KOTAKE_RED_POTION, gi);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a8c54c, EnTrt_GiveItem_BottledRedPotion);
PATCH_CALL(0x80ad4094, EnTrt_GiveItem_BottledRedPotion); /* En_Trt2 */
