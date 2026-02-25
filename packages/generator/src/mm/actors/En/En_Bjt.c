#include <combo.h>
#include <combo/item.h>
#include <combo/inventory.h>

static s32 sShouldReturnLetter = 0;

void EnBjt_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gi == GI_MM_HEART_PIECE)
    {
        npc = NPC_MM_TOILET_HEART_PIECE;
    }
    comboGiveItemNpc(actor, play, gi, npc, a, b);
    if (sShouldReturnLetter)
    {
        Inventory_UpdateBottleItem(play, ITEM_MM_RUTO_LETTER, GET_PLAYER(play)->heldItemButton);
        sShouldReturnLetter = 0;
    }
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
    case PLAYER_CUSTOM_IA_BOTTLE_RUTO_LETTER:
        gOotExtraFlags.rutoLetterUsed |= 2;
        Player_UpdateBottleHeld(gPlay, GET_PLAYER(gPlay), ITEM_MM_BOTTLE_EMPTY, PLAYER_IA_BOTTLE_EMPTY);
        if ((gOotExtraFlags.rutoLetterUsed & 3) != 3 && !Config_Flag(CFG_OOT_KZ_OPEN))
        {
            sShouldReturnLetter = 1;
        }
        break;
    }
}

PATCH_FUNC(0x80bfd434, EnBjt_RemoveItem);

s32 EnBjt_GetPlayerItemAction(PlayState* play)
{
    s32 itemAction = func_80123810(play);

    /* If it's ruto letter, return any paper. it's just used for comparison */
    if (itemAction == PLAYER_CUSTOM_IA_BOTTLE_RUTO_LETTER)
    {
        itemAction = PLAYER_IA_DEED_LAND;
    }

    return itemAction;
}

PATCH_CALL(0x80bfd738, EnBjt_GetPlayerItemAction);

static void EnBjt_MessageLetterReturn(PlayState* play)
{
    char* b;

    b = &play->msgCtx.font.textBuffer.schar[0x37];

    comboTextAppendStr(&b, TEXT_NL "You can also have the letter back..." TEXT_SIGNAL TEXT_END);
}

void EnBjt_AfterTextBox(Actor* this, PlayState* play, s16 messageId)
{
    switch (messageId)
    {
    case 0x294c:
        if (sShouldReturnLetter)
        {
            EnBjt_MessageLetterReturn(play);
        }
        break;
    }
}
