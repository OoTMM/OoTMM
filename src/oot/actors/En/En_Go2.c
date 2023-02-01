#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x180) = (h); } while (0)

static void EnGo2_HandlerNull(Actor* this, GameState_Play* play)
{
}

static void EnGo2_HandlerGiveBiggoronItem(Actor* this, GameState_Play* play)
{
    s16 gi = *(s16*)((char*)this + 0x584);

    if (Actor_HasParent(this))
    {
        SET_HANDLER(this, EnGo2_HandlerNull);
        return;
    }

    switch (gi)
    {
    case GI_OOT_PRESCRIPTION:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_TRADE_PRESCRIPTION, gi);
        comboRemoveTradeItemAdult(XITEM_OOT_ADULT_BROKEN_GORON_SWORD);
        break;
    case GI_OOT_CLAIM_CHECK:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_TRADE_CLAIM_CHECK, gi);
        comboRemoveTradeItemAdult(XITEM_OOT_ADULT_EYE_DROPS);
        break;
    case GI_OOT_SWORD_BIGGORON:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_TRADE_BIGGORON_SWORD, gi);
        gOotExtraFlags.biggoron = 1;
        break;
    }

    if (!(GET_LINK(play)->state & PLAYER_ACTOR_STATE_GET_ITEM))
        Message_Close(play);

    GiveItem(this, play, gi, 10000.f, 5000.f);
}

void EnGo2_SetBiggoronMessageId(Actor* this, GameState_Play* play, Actor* dst)
{
    u16 msgId;
    s16 gi;

    if ((this->variable & 0x1f) != 2)
        return;

    msgId = 0x3053;
    gi = -1;

    switch (GetActiveItem(play))
    {
    case 0xb: /* Broken Goron Sword */
        gi = GI_OOT_PRESCRIPTION;
        break;
    case 0xe: /* Eye Drops */
        gi = GI_OOT_CLAIM_CHECK;
        break;
    case 0xf: /* Claim check */
        if (!gOotExtraFlags.biggoron)
            gi = GI_OOT_SWORD_BIGGORON;
        else
            msgId = 0x3053;
        break;
    default:
        break;
    }

    if (gi >= 0)
    {
        *(s16*)((char*)this + 0x584) = gi;
        SET_HANDLER(this, EnGo2_HandlerGiveBiggoronItem);
    }

    this->messageId = msgId;
    dst->messageId = msgId;
}

PATCH_FUNC(0x80b58c8c, EnGo2_SetBiggoronMessageId);

int EnGo2_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    switch (gi)
    {
    case GI_OOT_TUNIC_GORON:
        gOotExtraFlags.tunicGoron = 1;
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GORON_LINK_TUNIC, gi);
        break;
    case GI_OOT_BOMB_BAG2:
    case GI_OOT_BOMB_BAG3:
        gi = comboOverride(OV_NPC, 0, NPC_OOT_GORON_BOMB_BAG, gi);
        break;
    }

    return GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x80b56ff4, EnGo2_GiveItem);
PATCH_CALL(0x80b5ad5c, EnGo2_GiveItem);

u16 EnGo2_GetMessageLinkGoron(void)
{
    if (!gOotExtraFlags.tunicGoron)
        return 0x3033;
    if (gSave.quest.medallionFire)
        return 0x3042;
    return 0x3038;
}

PATCH_FUNC(0x80b57868, EnGo2_GetMessageLinkGoron);

void EnGo2_InitWrapper(Actor* this, GameState_Play* play)
{
    ActorCallback callback;

    if (play->sceneId == SCE_OOT_BAZAAR && gSave.age == AGE_ADULT)
    {
        ActorDestroy(this);
    }
    else
    {
        callback = actorAddr(0x1ae, 0x80b5a0f0);
        callback(this, play);
    }
}
