#include <combo.h>

int EnBjt_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_TOILET_HEART_PIECE, gi);
    }
    return GiveItem(actor, play, gi, a, b);
}

void EnBjt_RemoveItem(s16 itemId)
{
    switch (itemId)
    {
    case ITEM_MM_DEED_LAND:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_LAND);
        break;
    case ITEM_MM_DEED_SWAMP:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_SWAMP);
        break;
    case ITEM_MM_DEED_MOUNTAIN:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_MOUNTAIN);
        break;
    case ITEM_MM_DEED_OCEAN:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_OCEAN);
        break;
    case ITEM_MM_LETTER_TO_KAFEI:
        comboRemoveTradeItem3(XITEM_MM_TRADE3_LETTER_TO_KAFEI);
        break;
    case ITEM_MM_LETTER_TO_MAMA:
        comboRemoveTradeItem2(XITEM_MM_TRADE2_LETTER_TO_MAMA);
        break;
    }
}

PATCH_FUNC(0x80bfd434, EnBjt_RemoveItem);
