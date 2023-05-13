#include <combo.h>
#include <combo/item.h>

static u8 sItemIndex;

static s16 kPrices[] = {
    20,
    200,
};

static s16 EnTab_GI(int index, int flags)
{
    s16 gi;
    s16 npc;
    int flag;

    switch (index)
    {
    case 0:
        gi = GI_MM_MILK;
        npc = NPC_MM_MILK_BAR_MILK;
        flag = gMmExtraFlags2.milkBarMilk;
        break;
    case 1:
        gi = GI_MM_CHATEAU;
        npc = NPC_MM_MILK_BAR_CHATEAU;
        flag = gMmExtraFlags2.milkBarChateau;
        break;
    default:
        UNREACHABLE();
        break;
    }

    gi = comboOverrideEx(OV_NPC, 0, npc, gi, flags);
    if (flag)
        gi = comboRenewable(gi, 0);
    if (!gi)
        return -1;
    return gi;
}

static void EnTab_TextItem(char** b, int index)
{
    s16 gi;

    gi = EnTab_GI(index, 0);
    if (gi < 0)
    {
        comboTextAppendStr(b, TEXT_COLOR_RED "SOLD OUT");
        comboTextAppendClearColor(b);
        comboTextAppendStr(b, TEXT_NL);
    }
    else
    {
        comboTextAppendItemName(b, gi, TF_PREPOS | TF_CAPITALIZE | TF_PROGRESSIVE);
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

    gi = EnTab_GI(sItemIndex, OVF_PROGRESSIVE);
    if (gi < 0)
        sc = SC_ERR_CANNOTBUY;
    else
        sc = comboItemPrecond(gi, kPrices[sItemIndex]);
    switch (sc)
    {
    case SC_OK:
    case SC_OK_NOCUTSCENE:
        GiveItem(this, play, gi, a, b);
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
