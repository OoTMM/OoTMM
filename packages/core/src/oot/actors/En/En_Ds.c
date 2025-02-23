#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/inventory.h>
#include <combo/shop.h>

#define PRICE (gComboConfig.prices[PRICES_OOT_MERCHANTS + 0x02])

void EnDs_GiveItem(Actor* actor, PlayState* play, s16 gi, float a, float b)
{
    if (!(GET_PLAYER(play)->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);
    comboRemoveTradeItemAdult(XITEM_OOT_ADULT_ODD_MUSHROOM);
    comboGiveItemNpc(actor, play, gi, NPC_OOT_TRADE_ODD_POTION, a, b);
}

PATCH_CALL(0x80aabcec, EnDs_GiveItem);
PATCH_CALL(0x80aabc6c, EnDs_GiveItem);

void EnDs_BluePotionItemQuery(ComboItemQuery* q)
{
    memset(q, 0, sizeof(ComboItemQuery));
    q->gi = GI_OOT_POTION_BLUE;
    q->ovType = OV_NPC;
    q->id = NPC_OOT_WITCH_BLUE_POTION;
    q->giRenew = GI_OOT_RECOVERY_HEART;
    if (BITMAP8_GET(gSharedCustomSave.oot.npc, NPC_OOT_WITCH_BLUE_POTION))
        q->ovFlags |= OVF_RENEW;
}

static void EnDs_BluePotionTextOverride(PlayState* play)
{
    char* b;
    ComboItemQuery q;

    EnDs_BluePotionItemQuery(&q);
    b = play->msgCtx.font.msgBuf;
    comboTextAppendHeader(&b);
    comboTextAppendItemNameQuery(&b, &q, TF_CAPITALIZE | TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "!" TEXT_NL "How about " TEXT_COLOR_RED);
    comboTextAppendNum(&b, PRICE);
    comboTextAppendStr(&b, " Rupees");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
}

void EnDs_TalkedTo(Actor* actor, PlayState* play, s16 textId)
{
    if (textId == 0x500c)
    {
        EnDs_BluePotionTextOverride(play);
    }
}

static void EnDs_MessageBoxCantBuy(PlayState* play, s16 textId)
{
    DisplayTextBox2(play, textId);
    comboTextMessageCantBuy(play, 0);
}

PATCH_CALL(0x80aac0b8, EnDs_MessageBoxCantBuy);

static int EnDs_CanBuy(void)
{
    ComboItemQuery q;
    int ret;

    EnDs_BluePotionItemQuery(&q);
    ret = comboItemPrecondEx(&q, PRICE);
    switch (ret)
    {
    case SC_ERR_NORUPEES:
        return 0;
    case SC_OK:
    case SC_OK_NOCUTSCENE:
        return 2;
    default:
        return 1;
    }
}

PATCH_FUNC(0x80aabf68, EnDs_CanBuy);

static void EnDs_PayBluePotion(void)
{
    AddRupeesRaw(-PRICE);
}

PATCH_CALL(0x80aac0d4, EnDs_PayBluePotion);

static void EnDs_GiveBluePotion(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Player* player;

    player = GET_PLAYER(play);
    if (player->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
        return;
    EnDs_BluePotionItemQuery(&q);
    comboGiveItem(this, play, &q, a, b);
}

PATCH_CALL(0x80aac10c, EnDs_GiveBluePotion);
PATCH_CALL(0x80aac004, EnDs_GiveBluePotion);

static int EnDs_AfterBuy(Actor* this, PlayState* play)
{
    if (!Actor_HasParentZ(this))
        return 0;
    BITMAP8_SET(gSharedCustomSave.oot.npc, NPC_OOT_WITCH_BLUE_POTION);
    return 1;
}

PATCH_CALL(0x80aabfc8, EnDs_AfterBuy);
