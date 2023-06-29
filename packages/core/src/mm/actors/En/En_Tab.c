#include <combo.h>
#include <combo/item.h>

static u8 sItemIndex;

static s16 kPrices[] = {
    20,
    200,
};

static void EnTab_ItemQuery(ComboItemQuery* q, int index)
{
    int flag;

    bzero(q, sizeof(*q));

    q->ovType = OV_NPC;
    q->ovFlags = OVF_PRECOND;

    switch (index)
    {
    case 0:
        q->gi = GI_MM_MILK;
        q->id = NPC_MM_MILK_BAR_MILK;
        flag = gMmExtraFlags2.milkBarMilk;
        break;
    case 1:
        q->gi = GI_MM_CHATEAU;
        q->id = NPC_MM_MILK_BAR_CHATEAU;
        flag = gMmExtraFlags2.milkBarChateau;
        break;
    default:
        UNREACHABLE();
        break;
    }

    if (flag)
        q->ovFlags |= OVF_RENEW;
}

static void EnTab_ItemOverride(ComboItemOverride* o, int index)
{
    ComboItemQuery q;

    EnTab_ItemQuery(&q, index);
    comboItemOverride(o, &q);
}

static void EnTab_TextItem(char** b, int index)
{
    ComboItemOverride o;

    EnTab_ItemOverride(&o, index);
    if (o.gi == 0)
    {
        comboTextAppendStr(b, TEXT_COLOR_RED "SOLD OUT");
        comboTextAppendClearColor(b);
        comboTextAppendStr(b, TEXT_NL);
    }
    else
    {
        comboTextAppendItemName(b, o.giRaw, TF_PREPOS | TF_CAPITALIZE | TF_PROGRESSIVE);
        comboTextAppendStr(b, ": " TEXT_COLOR_RED);
        comboTextAppendNum(b, kPrices[index]);
        comboTextAppendStr(b, " Rupees");
        comboTextAppendClearColor(b);
        comboTextAppendStr(b, TEXT_NL);
    }
}

static void EnTab_MessagePurchase(GameState_Play* play)
{
    char* b;

    b = play->textBuffer;
    comboTextAppendShopHeader2(&b, kPrices[0], kPrices[1]);

    comboTextAppendStr(&b, TEXT_CHOICE3);
    EnTab_TextItem(&b, 0);
    EnTab_TextItem(&b, 1);
    comboTextAppendStr(&b, "No Thanks" TEXT_END);
}

void EnTab_AfterTextBox(Actor* this, GameState_Play* play, s16 messageId)
{
    switch (messageId)
    {
    case 0x2b0b:
        EnTab_MessagePurchase(play);
        break;
    }
}

void EnTab_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    ComboItemOverride o;
    Actor_Player* link;
    int sc;

    link = GET_LINK(play);

    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    switch (gi)
    {
    case GI_MM_MILK:
        sItemIndex = 0;
        break;
    case GI_MM_CHATEAU:
        sItemIndex = 1;
        break;
    }

    EnTab_ItemQuery(&q, sItemIndex);
    comboItemOverride(&o, &q);
    if (gi == 0)
        sc = SC_ERR_CANNOTBUY;
    else
        sc = comboItemPrecondEx(&q, kPrices[sItemIndex]);

    switch (sc)
    {
    case SC_OK:
    case SC_OK_NOCUTSCENE:
        comboGiveItem(this, play, &q, a, b);
        break;
    default:
        sItemIndex = -1;
        this->attachedA = &link->base;
        PlayerDisplayTextBox(play, 0x2b0c, NULL);
        if (sc != SC_ERR_NORUPEES)
            comboTextMessageCantBuy(play, 0);
        break;
    }
}

void EnTab_AfterGivingItem(Actor* actor)
{
    switch (sItemIndex)
    {
    case 0:
        gMmExtraFlags2.milkBarMilk = 1;
        AddRupees(-kPrices[0]);
        break;
    case 1:
        gMmExtraFlags2.milkBarChateau = 1;
        AddRupees(-kPrices[1]);
        break;
    }
}
