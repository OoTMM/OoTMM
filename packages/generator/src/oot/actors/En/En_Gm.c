#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x25c) = (h); } while (0)
#define PRICE (gComboConfig.prices[PRICES_OOT_MERCHANTS + 0x00])
#define PRICE_KEG 50

static void EnGm_ItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(ComboItemQuery));

    q->ovType = OV_NPC;
    q->gi = GI_OOT_SWORD_KNIFE;
    q->id = NPC_OOT_MEDIGORON;

    if (BITMAP16_GET(gSave.info.eventsMisc, EV_OOT_INF_MEDIGORON))
    {
        q->ovType = OV_NONE;
        q->id = 0;
        q->gi = GI_OOT_POWDER_KEG;
    }
}

static void EnGm_Reset(Actor* this, u32 addr)
{
    SET_HANDLER(this, actorAddr(ACTOR_EN_GM, addr));
}

static void EnGm_GiveItemHandler2(Actor* this, PlayState* play)
{
    Player* player;
    ComboItemQuery q;

    if (Actor_HasParentZ(this))
    {
        BITMAP16_SET(gSave.info.eventsMisc, EV_OOT_INF_MEDIGORON);
        this->parent = NULL;
        EnGm_Reset(this, 0x80a9fd5c);
        return;
    }

    player = GET_PLAYER(play);
    if (player->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
        return;
    EnGm_ItemQuery(&q);
    comboGiveItem(this, play, &q, 9999.0f, 9999.0f);
}

static void EnGm_GiveItemHandler(Actor* this, PlayState* play)
{
    u16 price = BITMAP16_GET(gSave.info.eventsMisc, EV_OOT_INF_MEDIGORON) ? PRICE_KEG : PRICE;
    if (gSave.info.playerData.rupees < price)
    {
        DisplayTextBox2(play, 200);
        EnGm_Reset(this, 0x80a9fe98);
        return;
    }

    AddRupeesRaw(-price);
    SET_HANDLER(this, EnGm_GiveItemHandler2);
    EnGm_GiveItemHandler2(this, play);
}

PATCH_FUNC(0x80aa0020, EnGm_GiveItemHandler);

int EnGm_GetState(void)
{
    if (gSave.age == AGE_CHILD)
        return 0;

    if (BITMAP16_GET(gSave.info.eventsMisc, EV_OOT_INF_MEDIGORON) && !Config_Flag(CFG_OOT_POWDER_KEG))
        return 3;

    return 1;
}

PATCH_FUNC(0x80a9f8dc, EnGm_GetState);

static void hintMedigoron(PlayState* play)
{
    ComboItemQuery q;
    char* b;
    char* start;

    b = play->msgCtx.font.msgBuf;
    start = b;

    EnGm_ItemQuery(&q);
    comboTextAppendHeader(&b);
    comboTextAppendStr(&b, "How about buying ");
    comboTextAppendItemNameQuery(&b, &q, TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, " for " TEXT_COLOR_RED);
    comboTextAppendNum(&b, PRICE);
    comboTextAppendStr(&b, " Rupees");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Buy" TEXT_NL "Don't buy" TEXT_END);
    comboTextAutoLineBreaks(start);
}

static void textIntroKeg(char** b)
{
    comboTextAppendStr(b, "Hullo. Did ya come to buy a" TEXT_NL);
    comboTextAppendStr(b, TEXT_COLOR_RED "Powder Keg" TEXT_CZ "?");
    comboTextAppendStr(b, TEXT_BB);
}

static void textIntroBrag(char** b)
{
    comboTextAppendStr(b, "...This is just between us, but I" TEXT_NL);
    comboTextAppendStr(b, "have a huge bomb that is out" TEXT_NL);
    comboTextAppendStr(b, "of this world!");
    comboTextAppendStr(b, TEXT_BB);
}

static void textFinalKegSale(char** b)
{
    comboTextAppendStr(b, "How 'bout it?" TEXT_NL);
    comboTextAppendStr(b, TEXT_NL);
    comboTextAppendStr(b, TEXT_COLOR_GREEN TEXT_CHOICE2 "I'll buy one" TEXT_NL "No thanks" TEXT_END);
}

static void textOneKeg(char** b)
{
    comboTextAppendStr(b, TEXT_COLOR_RED "Powder Kegs" TEXT_CZ " are dangerous" TEXT_NL);
    comboTextAppendStr(b, "explosives, so you can carry only" TEXT_NL);
    comboTextAppendStr(b, "one at a time!" TEXT_END);
}

static void textTooHeavy(char** b)
{
    comboTextAppendStr(b, "Oh! But my product is so " TEXT_COLOR_RED "heavy" TEXT_CZ ", I" TEXT_NL);
    comboTextAppendStr(b, "don't think you can carry it." TEXT_BB);
    comboTextAppendStr(b, "I'm sorry I even brought it up..." TEXT_END);
}

static void textTooHeavyNoIntro(char** b)
{
    comboTextAppendStr(b, "Look, I'd like you to buy my bomb," TEXT_NL);
    comboTextAppendStr(b, "but it's just " TEXT_COLOR_RED "too heavy" TEXT_CZ " for you" TEXT_NL);
    comboTextAppendStr(b, "to carry." TEXT_NL);
    comboTextAppendStr(b, "Sorry." TEXT_END);
}

static void textKegSaleConversation(Actor* this, PlayState* play)
{
    char* b;

    b = play->msgCtx.font.msgBuf;

    u8* kegIntroSeen = ((u8*)this) + 0x2ad;

    comboTextAppendHeader(&b);

    s32 kegSale = 0;
    if (gOotExtraItems.bombSlot & 2)
    {
        if (gOotExtraAmmo.kegAmmo == 0)
        {
            if (*kegIntroSeen)
            {
                comboTextAppendStr(&b, "If you don't have a " TEXT_COLOR_RED "Powder Keg" TEXT_CZ "," TEXT_NL);
                comboTextAppendStr(&b, "I'll sell you one for " TEXT_COLOR_RED);
                comboTextAppendNum(&b, PRICE_KEG);
                comboTextAppendStr(&b, " Rupees" TEXT_CZ "!");
                comboTextAppendStr(&b, TEXT_BB);
                textFinalKegSale(&b);
                kegSale = 1;
            }
            else
            {
                textIntroKeg(&b);
                textIntroBrag(&b);
                comboTextAppendStr(&b, "So, won't ya buy a " TEXT_COLOR_RED "Powder Keg" TEXT_CZ TEXT_NL);
                comboTextAppendStr(&b, "for " TEXT_COLOR_RED);
                comboTextAppendNum(&b, PRICE_KEG);
                comboTextAppendStr(&b, " Rupees" TEXT_CZ "?");
                comboTextAppendStr(&b, TEXT_BB);
                textFinalKegSale(&b);
                kegSale = 1;
                *kegIntroSeen = 1;
            }
        }
        else
        {
            if (Player_GetStrength() == 3)
            {
                if (*kegIntroSeen)
                {
                    textOneKeg(&b);
                }
                else
                {
                    textIntroKeg(&b);
                    textIntroBrag(&b);
                    comboTextAppendStr(&b, "Oh, but you already have one.");
                    comboTextAppendStr(&b, TEXT_BB);
                    textOneKeg(&b);
                    *kegIntroSeen = 1;
                }
            }
            else
            {
                if (*kegIntroSeen)
                {
                    textTooHeavyNoIntro(&b);
                }
                else
                {
                    textIntroKeg(&b);
                    textIntroBrag(&b);
                    textTooHeavy(&b);
                    *kegIntroSeen = 1;
                }
            }
        }
    }
    else
    {
        if (Player_GetStrength() == 3)
        {
            if (*kegIntroSeen)
            {
                comboTextAppendStr(&b, "Look, I'd like you to buy my bomb," TEXT_NL);
                comboTextAppendStr(&b, "but it'd be irresponsible of me" TEXT_NL);
                comboTextAppendStr(&b, "to sell you one if you don't know" TEXT_NL);
                comboTextAppendStr(&b, "how to use it..." TEXT_END);
            }
            else
            {
                textIntroBrag(&b);
                comboTextAppendStr(&b, "Huh? Do you mean you don't know" TEXT_NL);
                comboTextAppendStr(&b, "how to use it, yet?");
                comboTextAppendStr(&b, TEXT_BB);
                comboTextAppendStr(&b, "Too bad! And I was hoping I could " TEXT_NL);
                comboTextAppendStr(&b, "sell one to you..." TEXT_END);
                *kegIntroSeen = 1;
            }
        }
        else
        {
            if (*kegIntroSeen)
            {
                textTooHeavyNoIntro(&b);
            }
            else
            {
                textIntroBrag(&b);
                textTooHeavy(&b);
                *kegIntroSeen = 1;
            }
        }
    }
    if (!kegSale)
    {
        EnGm_Reset(this, 0x80a9fe98);
    }
}

void EnGm_TalkedTo(Actor* this, PlayState* play)
{
    if (this->messageId == 0x304f)
    {
        if (BITMAP16_GET(gSave.info.eventsMisc, EV_OOT_INF_MEDIGORON))
        {
            textKegSaleConversation(this, play);
        }
        else
        {
            hintMedigoron(play);
        }
    }
}
