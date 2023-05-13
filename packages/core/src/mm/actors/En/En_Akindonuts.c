#include <combo.h>
#include <combo/item.h>

#define SCRUB_SWAMP             1
#define SCRUB_MOUNTAIN          2
#define SCRUB_OCEAN             3
#define SCRUB_VALLEY            4
#define SCRUB_BOMB_BAG          5
#define SCRUB_SHOP_BEANS        6
#define SCRUB_SHOP_POTION_GREEN 7
#define SCRUB_SHOP_POTION_BLUE  8

#define COND_NORUPEES   1
#define COND_CANTBUY    2
#define COND_OK         3

static const int kPrices[] = {
    10,
    200,
    40,
    100,
};

static const s16 kGI[] = {
    GI_MM_MAGIC_BEAN,
    GI_MM_BOMB_BAG3,
    GI_MM_POTION_GREEN,
    GI_MM_POTION_BLUE,
};

static int EnAkindonuts_ShopID(Actor* this)
{
    return this->variable & 0x3;
}

static s16 EnAkindonuts_GI(int id, int flags)
{
    int flag;
    int npc;
    s16 gi;

    switch (id)
    {
    case 0:
        npc = NPC_MM_SCRUB_SHOP_BEANS;
        flag = gMmExtraFlags2.scrubBeans;
        break;
    case 1:
        npc = NPC_MM_SCRUB_BOMB_BAG;
        flag = gMmExtraFlags.scrubBombBag;
        break;
    case 2:
        npc = NPC_MM_SCRUB_SHOP_POTION_GREEN;
        flag = gMmExtraFlags2.scrubPotionGreen;
        break;
    case 3:
        npc = NPC_MM_SCRUB_SHOP_POTION_BLUE;
        flag = gMmExtraFlags2.scrubPotionBlue;
        break;
    }

    gi = comboOverrideEx(OV_NPC, 0, npc, 0, flags);
    if (flag)
    {
        if (id == 1)
        {
            gi = GI_MM_RECOVERY_HEART;
        }
        else
        {
            gi = comboRenewable(gi, GI_MM_RECOVERY_HEART);
        }
    }

    return gi;
}

static s16 EnAkindonuts_ShopGI(Actor* this, int flags)
{
    int id;

    id = EnAkindonuts_ShopID(this);
    return EnAkindonuts_GI(id, flags);
}

static int EnAkindonuts_Precond(Actor* this)
{
    int id;
    int sc;
    s16 gi;

    id = EnAkindonuts_ShopID(this);
    gi = EnAkindonuts_ShopGI(this, OVF_PROGRESSIVE);
    sc = comboItemPrecond(gi, kPrices[id]);

    switch (sc)
    {
    case SC_OK:
    case SC_OK_NOCUTSCENE:
        *(s16*)((char*)this + 0x364) = -kPrices[id];
        *(s16*)((char*)this + 0x366) = kGI[id];
        return COND_OK;
    case SC_ERR_NORUPEES:
        return COND_NORUPEES;
    default:
        return COND_CANTBUY;
    }
}

PATCH_FUNC(0x80bed208, EnAkindonuts_Precond);
PATCH_FUNC(0x80bed27c, EnAkindonuts_Precond);
PATCH_FUNC(0x80bed2fc, EnAkindonuts_Precond);
PATCH_FUNC(0x80bed35c, EnAkindonuts_Precond);

static void EnAkindonuts_AlterMessageIntroSwamp(GameState_Play* play)
{
    s16 gi;
    char* b;
    char* start;

    gi = EnAkindonuts_GI(0, 0);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "I sell ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b,
        " to Deku Scrubs, but I'd really like to leave my hometown."
        TEXT_BB
        "I'm hoping to find some success in a livelier place!"
        TEXT_SIGNAL TEXT_END
    );
    comboTextAutoLineBreaks(start);
}

static void EnAkindonuts_AlterMessageIntroMountain(GameState_Play* play)
{
    s16 gi;
    char* b;
    char* start;

    gi = EnAkindonuts_GI(1, 0);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "I sell ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b,
        " but I'm focusing my marketing efforts on Gorons..."
        TEXT_BB
        "What I'd really like to do is go back home and do business where I'm surrounded by trees and grass."
        TEXT_SIGNAL TEXT_END
    );
    comboTextAutoLineBreaks(start);
}

static void EnAkindonuts_AlterMessageIntroOcean(GameState_Play* play)
{
    s16 gi;
    char* b;
    char* start;

    gi = EnAkindonuts_GI(2, 0);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "I'm selling ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b,
        ", but I'm focusing my marketing efforts on Zoras."
        TEXT_BB
        "Actually, I'd like to do business someplace where it's cooler and the air is clean."
        TEXT_SIGNAL TEXT_END
    );
    comboTextAutoLineBreaks(start);
}

static void EnAkindonuts_AlterMessageIntroIkana(GameState_Play* play)
{
    s16 gi;
    char* b;
    char* start;

    gi = EnAkindonuts_GI(3, 0);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "I'm here to sell ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b,
        "."
        TEXT_BB
        "Actually, I want to do business in the sea breeze while listening to the sound of the waves."
        TEXT_SIGNAL TEXT_END
    );
    comboTextAutoLineBreaks(start);
}

static void EnAkindonuts_AlterMessageBuy(Actor* this, GameState_Play* play)
{
    int id;
    int price;
    char* b;
    char* start;
    s16 gi;

    id = EnAkindonuts_ShopID(this);
    price = kPrices[id];
    gi = EnAkindonuts_GI(id, 0);
    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "I'll give you ");
    comboTextAppendItemName(&b, gi, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, " for ");
    comboTextAppendNum(&b, price);
    comboTextAppendStr(&b, " Rupees!" TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void EnAkindonuts_AlterMessageNoRupees(GameState_Play* play)
{
    char* b;
    char* start;

    b = play->textBuffer;
    comboTextAppendHeader(&b);
    start = b;
    comboTextAppendStr(&b, "Not enough Rupees!" TEXT_SIGNAL TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void EnAkindonuts_DisplayTextBox(GameState_Play* play, s16 messageId, Actor* this)
{
    PlayerDisplayTextBox(play, messageId, this);

    switch (messageId)
    {
    case 0x15e1:
        EnAkindonuts_AlterMessageIntroSwamp(play);
        break;
    case 0x15f5:
        EnAkindonuts_AlterMessageIntroMountain(play);
        break;
    case 0x1608:
        EnAkindonuts_AlterMessageIntroOcean(play);
        break;
    case 0x161c:
        EnAkindonuts_AlterMessageIntroIkana(play);
        break;
    case 0x15e9:
    case 0x15f3:
    case 0x1600:
    case 0x1606:
    case 0x1612:
    case 0x1617:
    case 0x1626:
    case 0x162d:
        EnAkindonuts_AlterMessageBuy(this, play);
        break;
    case 0x15ed:
    case 0x1603:
        EnAkindonuts_AlterMessageNoRupees(play);
        break;
    case 0x15ec:
    case 0x15ee:
    case 0x1601:
    case 0x1602:
    case 0x1613:
        comboTextMessageCantBuy(play);
        break;
    }
}

PATCH_CALL(0x80bed668, EnAkindonuts_DisplayTextBox);
PATCH_CALL(0x80bed88c, EnAkindonuts_DisplayTextBox);
PATCH_CALL(0x80bedb70, EnAkindonuts_DisplayTextBox);
PATCH_CALL(0x80bedd94, EnAkindonuts_DisplayTextBox);
PATCH_CALL(0x80bee058, EnAkindonuts_DisplayTextBox);
PATCH_CALL(0x80bee25c, EnAkindonuts_DisplayTextBox);
PATCH_CALL(0x80bee518, EnAkindonuts_DisplayTextBox);
PATCH_CALL(0x80bee724, EnAkindonuts_DisplayTextBox);

static u8 sScrubFlag;

int EnAkindonuts_HasGivenItem(Actor* this)
{
    int ret;

    ret = Actor_HasParent(this);
    if (ret)
    {
        switch (sScrubFlag)
        {
        case SCRUB_SWAMP:
            gMmExtraFlags.scrubSwamp = 1;
            break;
        case SCRUB_MOUNTAIN:
            gMmExtraFlags.scrubMountain = 1;
            break;
        case SCRUB_OCEAN:
            gMmExtraFlags.scrubOcean = 1;
            break;
        case SCRUB_VALLEY:
            gMmExtraFlags.scrubValley = 1;
            break;
        case SCRUB_BOMB_BAG:
            gMmExtraFlags.scrubBombBag = 1;
            break;
        case SCRUB_SHOP_BEANS:
            gMmExtraFlags2.scrubBeans = 1;
            break;
        case SCRUB_SHOP_POTION_GREEN:
            gMmExtraFlags2.scrubPotionGreen = 1;
            break;
        case SCRUB_SHOP_POTION_BLUE:
            gMmExtraFlags2.scrubPotionBlue = 1;
            break;
        }
        sScrubFlag = 0;
    }
    return ret;
}

PATCH_CALL(0x80bef3f4, EnAkindonuts_HasGivenItem);
PATCH_CALL(0x80bef38c, EnAkindonuts_HasGivenItem);

int EnAkindonuts_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_MM_DEED_SWAMP:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_LAND);
        if (gMmExtraFlags.scrubSwamp)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_SWAMP, gi);
            sScrubFlag = SCRUB_SWAMP;
        }
        break;
    case GI_MM_DEED_MOUNTAIN:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_SWAMP);
        if (gMmExtraFlags.scrubMountain)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_MOUNTAIN, gi);
            sScrubFlag = SCRUB_MOUNTAIN;
        }
        break;
    case GI_MM_DEED_OCEAN:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_MOUNTAIN);
        if (gMmExtraFlags.scrubOcean)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_OCEAN, gi);
            sScrubFlag = SCRUB_OCEAN;
        }
        break;
    case GI_MM_RUPEE_GOLD:
        comboRemoveTradeItem1(XITEM_MM_TRADE1_DEED_OCEAN);
        if (gMmExtraFlags.scrubValley)
            gi = GI_MM_RECOVERY_HEART;
        else
        {
            gi = comboOverride(OV_NPC, 0, NPC_MM_SCRUB_VALLEY, gi);
            sScrubFlag = SCRUB_VALLEY;
        }
        break;
    case GI_MM_MAGIC_BEAN:
        sScrubFlag = SCRUB_SHOP_BEANS;
        gi = EnAkindonuts_GI(0, OVF_PROGRESSIVE);
        break;
    case GI_MM_BOMB_BAG3:
        sScrubFlag = SCRUB_BOMB_BAG;
        gi = EnAkindonuts_GI(1, OVF_PROGRESSIVE);
        break;
    case GI_MM_POTION_GREEN:
        sScrubFlag = SCRUB_SHOP_POTION_GREEN;
        gi = EnAkindonuts_GI(2, OVF_PROGRESSIVE);
        break;
    case GI_MM_POTION_BLUE:
        sScrubFlag = SCRUB_SHOP_POTION_BLUE;
        gi = EnAkindonuts_GI(3, OVF_PROGRESSIVE);
        break;
    }

    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80bef3e4, EnAkindonuts_GiveItem);
PATCH_CALL(0x80bef434, EnAkindonuts_GiveItem);
