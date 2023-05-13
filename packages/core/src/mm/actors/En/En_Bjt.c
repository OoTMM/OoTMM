#include <combo.h>

void EnBjt_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_TOILET_HEART_PIECE, gi);
    }
    GiveItem(actor, play, gi, a, b);
}

void EnBjt_RemoveItem(s16 itemId)
{
    switch (itemId)
    {
    case 0x2b:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_LAND);
        break;
    case 0x2d:
        comboRemoveTradeItem3(XITEM_MM_TRADE3_LETTER_TO_KAFEI);
        break;
    case 0x2f:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_SWAMP);
        break;
    case 0x30:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_MOUNTAIN);
        break;
    case 0x31:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_OCEAN);
        break;
    case 0x33:
        comboRemoveTradeItem2(XITEM_MM_TRADE2_LETTER_TO_MAMA);
        break;
    }
}

PATCH_FUNC(0x80bfd434, EnBjt_RemoveItem);
