#include <combo.h>
#include <combo/item.h>
#include <combo/inventory.h>
#include <combo/shop.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x24c) = (h); } while (0)
#define PRICE (gComboConfig.prices[PRICES_OOT_MERCHANTS + 0x03])

int EnTa_GetActiveItem_Chicken(Actor* this, PlayState* play)
{
    int item;

    item = GetActiveItem(play);
    if (item == 3)
        comboRemoveTradeItemChild(XITEM_OOT_CHILD_CHICKEN);
    return item;
}

PATCH_CALL(0x809223d4, EnTa_GetActiveItem_Chicken);

void EnTa_GiveItem_MilkBottle(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_OOT_TALON_BOTTLE, a, b);
}

PATCH_CALL(0x80923cd8, EnTa_GiveItem_MilkBottle);
PATCH_CALL(0x80923d7c, EnTa_GiveItem_MilkBottle);

static void EnTa_BuyItemQuery(ComboItemQuery* q)
{
    memset(q, 0, sizeof(ComboItemQuery));
    q->gi = GI_OOT_MILK;
    q->ovType = OV_NPC;
    q->id = NPC_OOT_TALON_MILK;
    q->giRenew = GI_OOT_RECOVERY_HEART;
    if (BITMAP8_GET(gSharedCustomSave.oot.npc, NPC_OOT_TALON_MILK))
        q->ovFlags |= OVF_RENEW;
}

static void EnTa_BuyTextOverride(PlayState* play)
{
    char* b;
    ComboItemQuery q;

    EnTa_BuyItemQuery(&q);
    b = play->msgCtx.font.msgBuf;
    comboTextAppendHeader(&b);
    comboTextAppendItemNameQuery(&b, &q, TF_CAPITALIZE | TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "!" TEXT_NL "How about " TEXT_COLOR_RED);
    comboTextAppendNum(&b, PRICE);
    comboTextAppendStr(&b, " Rupees");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
}

void EnTa_TalkedTo(Actor* actor, PlayState* play, s16 textId)
{
    if (textId == 0x208b)
    {
        EnTa_BuyTextOverride(play);
    }
}

static void EnTa_Reset(Actor* this)
{
    void (*resetFunc)(Actor*, void*);
    void* arg;

    resetFunc = actorAddr(ACTOR_EN_TA, 0x80922cb4);
    arg = actorAddr(ACTOR_EN_TA, 0x809242bc);

    resetFunc(this, arg);
}

static void EnTa_BuyMilkHandler(Actor* this, PlayState* play)
{
    Player* player;
    ComboItemQuery q;

    if (Actor_HasParentZ(this))
    {
        this->parent = NULL;
        BITMAP8_SET(gSharedCustomSave.oot.npc, NPC_OOT_TALON_MILK);
        EnTa_Reset(this);
        return;
    }

    player = GET_PLAYER(play);
    if (player->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    EnTa_BuyItemQuery(&q);
    comboGiveItem(this, play, &q, 9999.f, 9999.f);
}

static void EnTa_BuyMilk(Actor* this, PlayState* play)
{
    ComboItemQuery q;
    int ret;

    EnTa_BuyItemQuery(&q);
    ret = comboItemPrecond(&q, PRICE);

    if (ret == SC_OK || ret == SC_OK_NOCUTSCENE)
    {
        AddRupeesRaw(-PRICE);
        SET_HANDLER(this, EnTa_BuyMilkHandler);
    }
    else
    {
        DisplayTextBox2(play, 0x85);
        if (ret != SC_ERR_NORUPEES)
            comboTextMessageCantBuy(play, 0);
        else
            comboTextMessageNoRupees(play, 0);
        EnTa_Reset(this);
    }
}

static void EnTa_BuyMilkPromptHandler(Actor* this, PlayState* play)
{
    if (Message_GetState(&play->msgCtx) != 4 || !Message_ShouldAdvance(play))
        return;

    if (play->msgCtx.choiceIndex == 1)
    {
        EnTa_Reset(this);
        return;
    }

    EnTa_BuyMilk(this, play);
}

PATCH_FUNC(0x80923d94, EnTa_BuyMilkPromptHandler);
