#include <combo.h>

#define SCRUB_ITEM_NUTS             0x00
#define SCRUB_ITEM_STICK            0x01
#define SCRUB_ITEM_HEART_PIECE      0x02
#define SCRUB_ITEM_SEEDS            0x03
#define SCRUB_ITEM_SHIELD_DEKU      0x04
#define SCRUB_ITEM_BOMBS            0x05
#define SCRUB_ITEM_SEEDS_ARROWS     0x06
#define SCRUB_ITEM_POTION_RED       0x07
#define SCRUB_ITEM_POTION_GREEN     0x08
#define SCRUB_ITEM_STICKS_UPGRADE   0x09
#define SCRUB_ITEM_NUTS_UPGRADE     0x0a

#define SCRUB_COND_NORUPEES          0x00
#define SCRUB_COND_CANTBUY           0x01
#define SCRUB_COND_OK                0x02

#define KEY(sceneId, inGrotto, item) \
    (((inGrotto) << 24) | ((sceneId) << 16) | (item))

static int EnDns_GetID(Actor* this)
{
    int key;
    int inGrotto;

    inGrotto = (gPlay->sceneId == SCE_OOT_GROTTOS);
    key = KEY(inGrotto ? gPlay->sceneId : gLastScene, inGrotto, this->variable & 0xff);
    switch (key)
    {
    /* Lost Woods */
    case KEY(SCE_OOT_LOST_WOODS, 0, SCRUB_ITEM_STICKS_UPGRADE): return 0x00;
    case KEY(SCE_OOT_LOST_WOODS, 0, SCRUB_ITEM_STICK):          return 0x01;
    case KEY(SCE_OOT_LOST_WOODS, 0, SCRUB_ITEM_NUTS):           return 0x02;
    case KEY(SCE_OOT_LOST_WOODS, 1, SCRUB_ITEM_NUTS_UPGRADE):   return 0x03;
    case KEY(SCE_OOT_LOST_WOODS, 1, SCRUB_ITEM_SEEDS_ARROWS):   return 0x04;

    /* Sacred Meadow */
    case KEY(SCE_OOT_SACRED_FOREST_MEADOW, 1, SCRUB_ITEM_POTION_GREEN):   return 0x05;
    case KEY(SCE_OOT_SACRED_FOREST_MEADOW, 1, SCRUB_ITEM_POTION_RED):     return 0x06;

    /* Hyrule Field */
    case KEY(SCE_OOT_HYRULE_FIELD, 1, SCRUB_ITEM_HEART_PIECE):   return 0x07;
    }

    return 0;
}

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
    int id;
    s16 gi;

    scrubData = *(void**)((char*)this + 0x2b0);
    gi = (s16)*(s32*)((char*)scrubData + 0x4);
    id = EnDns_GetID(this);
    gi = comboOverrideEx(OV_SCRUB, 0, id, gi, flags);
    return gi;
}

void EnDns_MaybeDestroy(Actor* this)
{
    s16 gi;

    if (gOotExtraScrubs & (1 << EnDns_GetID(this)))
    {
        /* Already bought from scrub */
        /* Use dummy GI to avoid having to look up the scrub data, as Shopnuts lack it */
        gi = comboOverrideEx(OV_SCRUB, 0, EnDns_GetID(this), 1, 0);
        if (!comboIsItemConsumable(gi))
            ActorDestroy(this);
    }
}

static int EnDns_CanBuy(Actor* this)
{
    s16 gi;
    s16 price;

    gi = EnDns_GetGi(this, 0);
    price = EnDns_GetPrice(this);

    switch (comboItemPrecond(gi, price))
    {
    case SC_OK:
    case SC_OK_NOCUTSCENE:
        return SCRUB_COND_OK;
    case SC_ERR_NORUPEES:
        return SCRUB_COND_NORUPEES;
    default:
        return SCRUB_COND_CANTBUY;
    }
}

PATCH_CALL(0x80a75620, EnDns_CanBuy);

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

static int EnDns_HasGivenItem(Actor* this, GameState_Play* play)
{
    int id;

    if (Actor_HasParent(this))
    {
        id = EnDns_GetID(this);
        gOotExtraScrubs |= (1 << id);
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80a75878, EnDns_HasGivenItem);

static int EnDns_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = EnDns_GetGi(actor, OVF_PROGRESSIVE);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a75734, EnDns_GiveItem);
PATCH_CALL(0x80a75750, EnDns_GiveItem);
PATCH_CALL(0x80a757a8, EnDns_GiveItem);
PATCH_CALL(0x80a757c4, EnDns_GiveItem);
PATCH_CALL(0x80a757e4, EnDns_GiveItem);
