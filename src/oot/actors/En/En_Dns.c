#include <combo.h>

static s16 EnDns_GetPrice(Actor* this)
{
    void* scrubData;
    s16 price;

    scrubData = *(void**)((char*)this + 0x2b0);
    price = *(s16*)((char*)scrubData + 0x0);

    return price;
}

static s16 EnDns_GetGi(Actor* this, int flags)
{
    void* scrubData;
    s16 gi;
    int npc;

    scrubData = *(void**)((char*)this + 0x2b0);
    gi = (s16)*(s32*)((char*)scrubData + 0x4);
    npc = -1;
    switch (this->variable)
    {
    case 2:
        npc = NPC_OOT_SCRUB_HP;
        break;
    case 9:
        npc = NPC_OOT_SCRUB_STICKS;
        break;
    case 10:
        npc = NPC_OOT_SCRUB_NUTS;
        break;
    }
    if (npc >= 0)
    {
        gi = comboOverrideEx(OV_NPC, 0, npc, gi, flags);
    }
    return gi;
}

static void EnDns_ShopText(Actor* this, GameState_Play* play)
{
    char* b;
    char* start;
    s16 price;
    s16 gi;

    b = play->msgCtx.textBuffer;
    start = b;
    price = EnDns_GetPrice(this);
    gi = EnDns_GetGi(this, 0);
    comboTextAppendShopHeader(&b, price);

    comboTextAppendStr(&b, "Do you want ");
    comboTextAppendItemName(&b, gi, TF_PROGRESSIVE);
    comboTextAppendStr(&b, " for " TEXT_COLOR_RED);
    comboTextAppendNum(&b, price);
    comboTextAppendStr(&b, " Rupees");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_NL TEXT_CHOICE2 TEXT_COLOR_GREEN);
    comboTextAppendStr(&b, "Yes" TEXT_NL);
    comboTextAppendStr(&b, "No" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static int EnDns_TalkedTo(Actor* this, GameState_Play* play)
{
    if (ActorTalkedTo(this))
    {
        EnDns_ShopText(this, play);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80a75510, EnDns_TalkedTo);

static int EnDns_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = EnDns_GetGi(actor, OVF_PROGRESSIVE | OVF_DOWNGRADE);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a75734, EnDns_GiveItem);
PATCH_CALL(0x80a75750, EnDns_GiveItem);
PATCH_CALL(0x80a757a8, EnDns_GiveItem);
PATCH_CALL(0x80a757c4, EnDns_GiveItem);
PATCH_CALL(0x80a757e4, EnDns_GiveItem);
