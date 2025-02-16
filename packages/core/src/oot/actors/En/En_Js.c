#include <combo.h>
#include <combo/item.h>
#include <combo/text.h>
#include <combo/shop.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x27c) = (h); } while (0)
#define PRICE (gComboConfig.prices[PRICES_OOT_MERCHANTS + 0x01])

static void EnJs_ItemQuery(ComboItemQuery* q)
{
    memset(q, 0, sizeof(ComboItemQuery));
    q->gi = GI_OOT_BOMBCHU_20;
    q->ovType = OV_NPC;
    q->id = NPC_OOT_CARPET_MERCHANT;
    q->giRenew = GI_OOT_RECOVERY_HEART;
    if (BITMAP8_GET(gSharedCustomSave.oot.npc, NPC_OOT_CARPET_MERCHANT))
        q->ovFlags |= OVF_RENEW;
}

static void EnJs_TextOverride(PlayState* play)
{
    char* b;
    ComboItemQuery q;

    EnJs_ItemQuery(&q);
    b = play->msgCtx.font.msgBuf;
    comboTextAppendHeader(&b);
    comboTextAppendItemNameQuery(&b, &q, TF_CAPITALIZE | TF_PREPOS | TF_PROGRESSIVE);
    comboTextAppendStr(&b, "!" TEXT_NL "How about " TEXT_COLOR_RED);
    comboTextAppendNum(&b, PRICE);
    comboTextAppendStr(&b, " Rupees");
    comboTextAppendClearColor(&b);
    comboTextAppendStr(&b, "?" TEXT_NL TEXT_COLOR_GREEN TEXT_CHOICE2 "Yes" TEXT_NL "No" TEXT_END);
}

void EnJs_TalkedTo(Actor* actor, PlayState* play, s16 textId)
{
    if (textId == 0x6077)
    {
        EnJs_TextOverride(play);
    }
}

static void EnJs_HandleBuyGetItem(Actor* this, PlayState* play)
{
    Player* player;
    ComboItemQuery q;

    if (Actor_HasParentZ(this))
    {
        BITMAP8_SET(gSharedCustomSave.oot.npc, NPC_OOT_CARPET_MERCHANT);
        *(int*)((char*)this + 0x118) = 0;
        SET_HANDLER(this, actorAddr(ACTOR_EN_JS, 0x80adb868));
        return;
    }

    player = GET_PLAYER(play);
    if (player->stateFlags1 & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    EnJs_ItemQuery(&q);
    comboGiveItem(this, play, &q, 9999.f, 9999.f);
}

static void EnJs_HandleBuy(Actor* this, PlayState* play)
{
    void (*EnJs_CancelBuy)(Actor*);
    ComboItemQuery q;
    int ret;

    EnJs_ItemQuery(&q);
    ret = comboItemPrecondEx(&q, PRICE);

    if (ret == SC_OK || ret == SC_OK_NOCUTSCENE)
    {
        AddRupeesRaw(-PRICE);
        SET_HANDLER(this, EnJs_HandleBuyGetItem);
    }
    else
    {
        DisplayTextBox2(play, 0x6074);
        if (ret != SC_ERR_NORUPEES)
            comboTextMessageCantBuy(play, 0);
        EnJs_CancelBuy = actorAddr(ACTOR_EN_JS, 0x80adb554);
        EnJs_CancelBuy(this);
    }
}

PATCH_FUNC(0x80adb6b8, EnJs_HandleBuy);
