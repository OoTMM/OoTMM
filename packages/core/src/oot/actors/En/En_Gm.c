#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x25c) = (h); } while (0)
#define PRICE (gComboConfig.prices[PRICES_OOT_MERCHANTS + 0x00])

static void EnGm_ItemQuery(ComboItemQuery* q)
{
    bzero(q, sizeof(ComboItemQuery));

    q->ovType = OV_NPC;
    q->gi = GI_OOT_SWORD_KNIFE;
    q->id = NPC_OOT_MEDIGORON;
}

static void EnGm_Reset(Actor* this)
{
    SET_HANDLER(this, actorAddr(ACTOR_EN_GM, 0x80a9fd5c));
}

static void EnGm_GiveItemHandler2(Actor* this, PlayState* play)
{
    Player* player;
    ComboItemQuery q;

    if (Actor_HasParentZ(this))
    {
        BITMAP16_SET(gSave.info.eventsMisc, EV_OOT_INF_MEDIGORON);
        EnGm_Reset(this);
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
    if (gSave.info.playerData.rupees < PRICE)
    {
        DisplayTextBox2(play, 200);
        EnGm_Reset(this);
        return;
    }

    AddRupeesRaw(-PRICE);
    SET_HANDLER(this, EnGm_GiveItemHandler2);
    EnGm_GiveItemHandler2(this, play);
}

PATCH_FUNC(0x80aa0020, EnGm_GiveItemHandler);

int EnGm_GetState(void)
{
    if (gSave.age == AGE_CHILD)
        return 0;

    if (BITMAP16_GET(gSave.info.eventsMisc, EV_OOT_INF_MEDIGORON))
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

void EnGm_TalkedTo(Actor* this, PlayState* play)
{
    if (this->messageId == 0x304f)
        hintMedigoron(play);
}
